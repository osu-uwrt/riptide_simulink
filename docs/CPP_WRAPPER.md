# C++ model wrapper

A header-only library for supervising generated model nodes from an overseer: checking they're up, reading and setting their parameters, and reloading configuration. The wrapper talks to the model's **remote** ROS process. It doesn't expose that process's `rclcpp::Node`.

## Linking

```cmake
find_package(riptide_simulink REQUIRED)
target_link_libraries(my_overseer riptide_simulink::model)
```

```cpp
#include <riptide_simulink/simulink_model.hpp>
```

Add `riptide_simulink` as a `<depend>` in your `package.xml`. [overseer.cpp](../examples/overseer.cpp) is a complete example.

## Construction

Construct `SimulinkModel` with:

- your overseer's `rclcpp::Node::SharedPtr`,
- the model node's fully qualified name (preferred), or a basename that exactly one node uses,
- options,
- an optional parameter-provider callback.

## Status and requests

- `status()` reports available, ready or ambiguous.
- `list_parameters`, `get_parameters`, `set_parameters` and `reload_parameters` return whether the request was **accepted**. Completion callbacks report the actual outcome.
- Only a successful reload of the required parameters marks the model ready.
- `on_state_change` and `on_reload` let a specialized overseer react.
- The optional Trigger reload service acknowledges that a reload was scheduled, not that it finished.

## Retries and timing

- A failed initial parameter reload is retried up to three times, one second apart by default (`reload_attempts`, `retry_period`). This covers nodes whose parameters appear after their services. An explicit reload starts a fresh attempt budget, and losing the node resets it.
- The request queue holds ten operations by default. Discovery polls every 500 ms, and requests time out after three seconds on a steady clock.
- The wrapper watches for the node or its services disappearing, then reconnects and reapplies configuration. A restart that completes entirely between two graph checks can't be detected without a model-specific heartbeat, so call reload when your overseer detects such a restart.

## Threading

A mutually exclusive callback group plus synchronized state makes the wrapper safe with single- and multi-threaded executors. Callbacks and providers must:

- return promptly,
- never spin or wait on the executor,
- never destroy the wrapper from inside its own callbacks.

## Parameter files

- `load_ros_parameters(file, node_fqn)` reads standard ROS parameter YAML and keeps native types.
- `load_legacy_parameters(file, options)` reads the old overseer format: nested names joined with `__`, and numbers scaled (by 1,000,000 by default, truncating toward zero as the old overseer did).

In both, booleans stay booleans, quoted scalars stay strings, and mixed or empty arrays and overflow are errors. Keep computed or robot-specific values, such as wrench matrices and thruster state, in the provider callback rather than in files.
