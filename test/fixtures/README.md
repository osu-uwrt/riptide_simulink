# R2023a ROS fixture

`models/riptide_smoke.slx` is an editable R2023a model containing a ROS 2 Blank Message connected to Publish, plus a Constant/Terminator branch. It uses built-in messages and requires no external project or setup function. It was created through `model_edit` and checked with `model_read` and `model_check`; connectivity checks were healthy.

It is excluded from default all-model selection but can be selected explicitly:

```bash
python3 -m riptide_simulink.cli build riptide_smoke --workspace test/fixtures \
  --source local --jobs 1 --archives-dir /tmp/riptide-fixture-artifacts \
  --no-process-local --no-process-deploy
```

Generation must not change the `.slx` checksum. Test development/release staging in separate temporary workspaces to avoid colliding with existing generated packages. The archive configuration is applied temporarily by the common MATLAB backend; the saved fixture remains unchanged.
