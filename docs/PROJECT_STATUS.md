# Project status and completion checklist

This is a staged firmware foundation, not a finished AI sorter. Track implemented work separately from work requiring hardware, data or validation.

## Implemented in repository
- [x] ESP-IDF project structure and ESP32-S3 target default.
- [x] Sorter state machine with object debounce and review/fault states.
- [x] Servo PWM manager and provisional four-class angle mapping.
- [x] Classifier interface with fail-closed UNKNOWN stub.
- [x] Camera manager interface using esp32-camera; refuses initialization while GPIOs are unset.
- [x] Dataset folder layout checker and starter unit tests.
- [x] AI model integration contract and hardware/camera bring-up checklists.
- [x] GitHub Actions ESP-IDF build workflow.

## Not yet implemented / not verified
- [ ] CI build has passed. Check the Actions tab; a workflow file is not evidence of a successful build.
- [ ] Exact board and camera module selected and purchased.
- [ ] Camera pin map configured and capture tested on-device.
- [ ] Real labeled dataset collected and reviewed.
- [ ] Trained/quantized on-device model included and inference adapter implemented.
- [ ] Camera JPEG decode, resize and normalization implemented.
- [ ] Model latency, RAM/PSRAM use, flash use and held-out accuracy measured.
- [ ] Physical diverter angles calibrated with the actual chute.
- [ ] Exit sensor added to verify item movement.
- [ ] Full offline end-to-end test completed.

## Definition of done for a demonstrator
1. Firmware build passes on the selected ESP-IDF version.
2. Camera captures reliably for at least 100 consecutive trials.
3. A held-out dataset report includes per-class precision/recall and confusion matrix.
4. Invalid/unknown/low-confidence inference does not actuate the diverter.
5. Each valid class routes correctly in repeated physical tests.
6. Jam, sensor stuck, camera failure and model failure lead to safe stop/review.
7. No network connection is required during classification.

## Current limitation
The classifier intentionally returns UNKNOWN. Do not present this repository as recognizing waste yet. Exact hardware, pin map and dataset remain decision gates.
