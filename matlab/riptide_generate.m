function riptide_generate(requestFile)
    % Shared R2023a ROS 2 source-generation entry point. Never saves input models.

    request = jsondecode(fileread(requestFile));
    if ~strcmp(['R' version('-release')], request.matlab)
        error('riptide:Version', 'Requested %s; running R%s.', request.matlab, version('-release'));
    end

    oldPath = path;
    oldDir = pwd;
    oldFiles = Simulink.fileGenControl('getConfig');

    % default the compiler job count to the available cpus
    if ~isfield(request, 'jobs')
        [status, count] = system('nproc');
        if status ~= 0, error('riptide:Jobs', 'Cannot determine CPU count; supply request.jobs'); end
        request.jobs = str2double(strtrim(count));
    end
    validateattributes(request.jobs, {'numeric'}, {'scalar','integer','positive'});

    oldMake = getenv('MAKEFLAGS');
    oldParallel = getenv('CMAKE_BUILD_PARALLEL_LEVEL');
    oldDefaults = getenv('COLCON_DEFAULTS_FILE');
    envCleanup = onCleanup(@() restoreBuildEnv(oldMake, oldParallel, oldDefaults)); %#ok<NASGU>

    % limit the build parallelism and keep colcon sequential
    setenv('MAKEFLAGS', sprintf('-j%d -l%d', request.jobs, request.jobs));
    setenv('CMAKE_BUILD_PARALLEL_LEVEL', num2str(request.jobs));

    defaultsFile = fullfile(fileparts(requestFile), 'colcon-defaults.yaml');
    fid = fopen(defaultsFile, 'w');
    fprintf(fid, 'build:\n  executor: sequential\n');
    fclose(fid);
    setenv('COLCON_DEFAULTS_FILE', defaultsFile);

    cleanup = onCleanup(@() restoreEnvironment(oldPath, oldDir, oldFiles)); %#ok<NASGU>

    if ~isfolder(request.output), mkdir(request.output); end

    % generate every requested model
    results = struct('model', {}, 'architecture', {}, 'archive', {});
    for i = 1:numel(request.models)
        item = request.models(i);
        itemResults = generateModel(item, request, fullfile(request.output, sprintf('model_%d', i)));
        results = [results itemResults]; %#ok<AGROW>
    end

    % write the archive list for the python side
    fid = fopen(fullfile(request.output, 'result.json'), 'w');
    fileCleanup = onCleanup(@() fclose(fid)); %#ok<NASGU>
    fprintf(fid, '%s', jsonencode(results));
end


function results = generateModel(item, request, modelOutput)
    modelPath = path;
    modelDirectory = pwd;
    modelEnvironment = onCleanup(@() restoreModelEnv(modelPath, modelDirectory)); %#ok<NASGU>
    results = struct('model', {}, 'architecture', {}, 'archive', {});

    project = [];
    if ~isempty(item.project)
        project = openProject(item.project);
    end
    projectCleanup = onCleanup(@() closeProject(project)); %#ok<NASGU>

    % make the model and its configurations visible
    modelDir = fileparts(item.model);
    addpath(modelDir, item.config_base);
    cd(modelDir);

    if ~isempty(item.setup)
        if isempty(regexp(item.setup, '^[A-Za-z]\w*(\.[A-Za-z]\w*)*$', 'once'))
            error('riptide:Setup', 'setup must name a MATLAB function, not an expression');
        end
        feval(item.setup);
    end

    fprintf('RIPTIDE_PROGRESS %s\n', jsonencode(struct('model', item.model, 'phase', 'Preparing messages')));
    prepareMessages(item);

    if strcmp(request.operation, 'messages'), return; end
    if strcmp(request.operation, 'open')
        % A desktop process is launched separately by the Python open operation.
        open_system(item.model);
        return;
    end

    [~, model] = fileparts(item.model);
    if bdIsLoaded(model)
        error('riptide:OpenModel', 'Build process already has model %s loaded; refusing to discard state', model);
    end

    load_system(item.model);
    modelCleanup = onCleanup(@() close_system(model, 0)); %#ok<NASGU>

    if strcmp(get_param(model, 'BlockDiagramType'), 'library')
        error('riptide:Library', '%s is a library, not a standalone ROS model', model);
    end

    % remember the active configuration so it can be restored
    original = getActiveConfigSet(model);
    originalName = original.Name;
    if isa(original, 'Simulink.ConfigSetRef'), original = getRefConfigSet(original); end

    for j = 1:numel(request.architectures)
        arch = request.architectures{j};
        fprintf('RIPTIDE_PROGRESS %s\n', jsonencode(struct('model', item.model, ...
            'architecture', arch, 'phase', sprintf('Generating (%d/%d)', j, numel(request.architectures)))));

        % start from the consumer configuration or the model's own
        if isfield(item.configurations, arch)
            factory = item.configurations.(arch);
            if isempty(regexp(factory, '^[A-Za-z]\w*(\.[A-Za-z]\w*)*$', 'once'))
                error('riptide:Config', 'Configuration override must name a function');
            end
            config = copy(feval(factory));
        else
            config = copy(original);
        end

        name = ['RIPTIDE_' arch];
        config.Name = name;
        config = riptide_ert_config(config);

        set_param(config, 'HardwareBoard', 'Robot Operating System 2 (ROS 2)', ...
            'TargetLang', 'C++', 'Toolchain', 'Colcon Tools', 'GenCodeOnly', 'on', ...
            'MatFileLogging', 'off', 'CodeInterfacePackaging', 'C++ class');

        % select the hardware for the architecture
        if strcmp(arch, 'arm64')
            set_param(config, 'ProdHWDeviceType', 'ARM Compatible->ARM 64-bit (LP64)');
        elseif strcmp(arch, 'x86_64')
            set_param(config, 'ProdHWDeviceType', 'Intel->x86-64 (Linux 64)');
        else
            error('riptide:Architecture', 'Unsupported architecture %s', arch);
        end

        set_param(config, 'ProdEqTarget', 'on');
        % Keep the model's ROS settings, changing only generation/deployment settings.
        target = get_param(config, 'CoderTargetData');
        target.ROS2Install.Folder = ['/opt/ros/' request.ros];
        % In R2023a RemoteBuild selects portable .tgz packaging. GenCodeOnly
        % prevents a remote connection/build; the local mode produces no archive.
        target.ROS.RemoteBuild = true;
        target.Runtime.BuildAction = 'Build';
        set_param(config, 'CoderTargetData', target);

        % build with the temporary configuration
        attachConfigSet(model, config);
        setActiveConfigSet(model, name);
        configCleanup = onCleanup(@() restoreConfig(model, originalName, name));

        output = fullfile(modelOutput, arch);
        mkdir(output);
        % A persistent build cache lets Simulink skip or reuse unchanged work.
        codegen = output;
        if isfield(item, 'build_cache') && ~isempty(item.build_cache)
            codegen = fullfile(item.build_cache, arch);
        end
        Simulink.fileGenControl('set', 'CacheFolder', fullfile(codegen, 'cache'), ...
            'CodeGenFolder', codegen, 'createDir', true);
        % ROS Toolbox archives are emitted in the current working directory.
        cd(output);
        % Remove stale in-place archives so a freshly generated one is unambiguous.
        removeArchives(codegen, model);
        % The last good archive is set aside during the build. Simulink skips
        % generation, and writes no archive, when the cached code is up to date.
        stored = fullfile(codegen, 'riptide-last.tgz');
        pending = [stored '.pending'];

        if isfile(pending), delete(pending); end
        if isfile(stored), movefile(stored, pending); end

        % build, reusing the cached archive when nothing changed
        buildModel(model, output, false);
        archives = findArchives(output, codegen, model);
        reused = isempty(archives) && isfile(pending);
        if reused
            fprintf('%s %s is up to date; reusing its cached archive.\n', model, arch);
            movefile(pending, stored);
            copyfile(stored, fullfile(output, [model '.tgz']));
            archives = {fullfile(output, [model '.tgz'])};
        elseif isempty(archives)
            % Up to date but without a trusted archive (e.g. an interrupted build).
            buildModel(model, output, true);
            archives = findArchives(output, codegen, model);
        end
        if isfile(pending), delete(pending); end

        if numel(archives) ~= 1
            error('riptide:Archive', 'Expected one generated archive for %s; found %d', model, numel(archives));
        end
        if ~strcmp(codegen, output)
            copyfile(archives{1}, stored);
        end

        results(end+1) = struct('model', item.model, 'architecture', arch, ...
            'archive', archives{1}); %#ok<AGROW>
        fprintf('RIPTIDE_PROGRESS %s\n', jsonencode(struct('model', item.model, ...
            'architecture', arch, 'phase', 'Generation complete', 'cached', reused)));

        clear configCleanup
    end
end


function prepareMessages(item)
    if isempty(item.messages), return; end
    cache = item.message_cache;
    marker = fullfile(cache, '.complete');
    ownership = fullfile(cache, '.riptide-simulink-cache');
    if isfolder(cache) && ~isfile(ownership)
        error('riptide:Cache', 'Refusing to replace unowned message cache: %s', cache);
    end

    % regenerate the message cache when it is incomplete
    if ~isfile(marker)
        if isfolder(cache), rmdir(cache, 's'); end
        mkdir(cache);
        fclose(fopen(ownership, 'w'));
        for k = 1:numel(item.messages)
            source = item.messages{k};
            xml = xmlread(fullfile(source, 'package.xml'));
            name = char(xml.getElementsByTagName('name').item(0).getTextContent());
            copyfile(source, fullfile(cache, name));
        end
        ros2genmsg(cache);
        fclose(fopen(marker, 'w'));
    end

    genDir = fullfile(cache, 'matlab_msg_gen', computer('arch'));
    addpath(fullfile(genDir, 'install', 'm'));
    % Re-register cached definitions when switching between message revisions.
    info = load(fullfile(genDir, 'src', 'VersionInfo.mat'), 'versionStorageMap');
    v = info.versionStorageMap;
    ros.internal.custommsgs.updatePreferences(v('MessageList'), v('ServiceRequestList'), ...
        v('ServiceResponseList'), v('ServiceList'), v('ActionList'), 'ros2', genDir);
    ros.internal.utilities.getPersistentMapForROS2('clear');
end


function buildModel(model, output, force)
    diagnostics = fullfile(output, 'diagnostics.log');
    sldiagviewer.diary(diagnostics);
    diagnosticCleanup = onCleanup(@() sldiagviewer.diary('off'));

    try
        if force
            slbuild(model, 'ForceTopModelBuild', true);
        else
            slbuild(model);
        end
    catch buildError
        clear diagnosticCleanup
        % Batch builds otherwise hide the useful MATLAB Function diagnostic
        % behind a generic tlc_c error. Emit it before temporary files go away.
        if isfile(diagnostics), fprintf(2, '%s\n', fileread(diagnostics)); end
        rethrow(buildError);
    end
end


function archives = findArchives(output, codegen, model)
    found = dir(fullfile(output, '**', [model '.tgz']));
    if ~strcmp(codegen, output)
        found = [found; dir(fullfile(codegen, '**', [model '.tgz']))];
    end

    archives = unique(arrayfun(@(f) fullfile(f.folder, f.name), found, 'UniformOutput', false));
end


function removeArchives(folder, model)
    stale = dir(fullfile(folder, '**', [model '.tgz']));
    for k = 1:numel(stale)
        delete(fullfile(stale(k).folder, stale(k).name));
    end
end


function restoreConfig(model, original, temporary)
    if bdIsLoaded(model)
        setActiveConfigSet(model, original);
        detachConfigSet(model, temporary);
    end
end


function closeProject(project)
    if ~isempty(project), close(project); end
end


function restoreEnvironment(oldPath, oldDir, oldFiles)
    Simulink.fileGenControl('setConfig', 'config', oldFiles);
    cd(oldDir);
    path(oldPath);
end


function restoreBuildEnv(make, parallel, defaults)
    setenv('MAKEFLAGS', make);
    setenv('CMAKE_BUILD_PARALLEL_LEVEL', parallel);
    setenv('COLCON_DEFAULTS_FILE', defaults);
end


function restoreModelEnv(previousPath, previousDirectory)
    cd(previousDirectory);
    path(previousPath);
end
