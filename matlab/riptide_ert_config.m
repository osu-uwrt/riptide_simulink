function config = riptide_ert_config(config)
    % Switch a copied configuration to ERT without accepting its reset defaults.
    % Calling switchTarget even for ERT -> ERT resets user settings in R2023a.

    if strcmp(get_param(config, 'SystemTargetFile'), 'ert.tlc'), return; end

    metadata = get_param(config, 'ObjectParameters');
    names = fieldnames(metadata);

    % These describe the target and its generated executable scaffolding.
    targetSettings = {'SystemTargetFile', 'TemplateMakefile', 'MakeCommand', ...
        'IsERTTarget', 'GenerateSampleERTMain', 'ModelStepFunctionPrototypeControlCompliant'};

    % save every writable setting that is not part of the target
    saved = struct;
    for i = 1:numel(names)
        name = names{i};
        if ismember(name, targetSettings) || any(strcmp(metadata.(name).Attributes, 'read-only'))
            continue;
        end

        saved.(name) = get_param(config, name);
    end

    config.switchTarget('ert.tlc', '');

    names = fieldnames(saved);

    % Related parameters can enable each other; retry before verifying every value.
    for pass = 1:2
        for i = 1:numel(names)
            name = names{i};
            try
                if ~isequaln(get_param(config, name), saved.(name))
                    set_param(config, name, saved.(name));
                end
            catch
                % The final check reports any unsupported or unrestored setting.
            end
        end
    end

    % verify that every saved setting survived the target switch
    for i = 1:numel(names)
        name = names{i};
        try
            unchanged = isequaln(get_param(config, name), saved.(name));
        catch
            unchanged = false;
        end

        if ~unchanged
            error('riptide:ConfigConversion', ['Cannot preserve configuration parameter %s ' ...
                'when changing targets. Supply an explicit ERT configuration factory.'], name);
        end
    end
end
