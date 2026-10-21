function test_configuration
    % Run in R2023a with this package's matlab/ directory on the path.

    % build a configuration with non-default consumer settings
    original = Simulink.ConfigSet;
    set_param(original, 'Solver', 'ode3', 'FixedStep', '0.02', ...
        'CustomInclude', '/tmp/consumer_includes', 'MATLABDynamicMemAlloc', 'on', ...
        'SupportVariableSizeSignals', 'on', 'DefaultParameterBehavior', 'Tunable');
    snapshot = copy(original);

    % converting a copy must not touch the source and must select ert
    configured = riptide_ert_config(copy(original));
    assert(isequal(original, snapshot), 'Source configuration changed');
    assert(strcmp(get_param(configured, 'SystemTargetFile'), 'ert.tlc'));

    % consumer settings must survive the conversion
    protected = {'Solver', 'FixedStep', 'CustomInclude', 'MATLABDynamicMemAlloc', ...
        'SupportVariableSizeSignals', 'DefaultParameterBehavior'};
    for i = 1:numel(protected)
        assert(isequal(get_param(configured, protected{i}), get_param(original, protected{i})), ...
            'Consumer setting changed: %s', protected{i});
    end

    % converting an already ert configuration must be a no-op
    alreadyERT = copy(configured);
    configured = riptide_ert_config(configured);
    assert(isequal(configured, alreadyERT), 'Existing ERT settings changed');

    disp('Configuration preservation checks passed');
end
