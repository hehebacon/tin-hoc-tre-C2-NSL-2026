# C2 Smart Waste Sorter — ESP32-S3 Offline AI

Firmware scaffold for a camera-based waste sorter. The intended device captures an item, classifies it locally (without internet), and controls a servo diverter.

## Current status

- ESP-IDF project scaffold and fail-closed sorting state machine.
- ESP32 camera component dependency and a guarded camera capture interface.
- Dataset layout checker: `python3 tools/check_dataset.py`.
- AI classifier intentionally returns UNKNOWN until a real trained model is integrated.
- Camera GPIOs are unset (`-1`) until the exact board and camera module are selected.
- Not production-ready: compile, camera integration, AI inference and hardware tests remain.

## Architecture

Camera frame -> image preprocessing -> on-device classifier -> confidence gate -> sorter state machine -> servo diverter.

## Proposed first prototype

- ESP32-S3 with PSRAM (N8R8-class board is a candidate, not yet purchased)
- Compatible camera module with a verified pin map
- One servo diverter, external servo supply, object sensor
- Optional chute/end-stop sensor to verify the item moved
- Four provisional classes: paper, plastic, metal, other

Confirm the official competition requirements before fixing labels or buying hardware.

## Build

Use ESP-IDF 5.x. From the project root:

```sh
idf.py set-target esp32s3
idf.py build
```

The component manager will fetch the camera dependency declared in `main/idf_component.yml`. This repository has not yet been built against a real ESP-IDF environment, so build success is not claimed.

Review these documents first:
- `docs/HARDWARE.md`
- `docs/CAMERA_BRINGUP.md`
- `main/perception/AI_MODEL_PIPELINE.md`
- `docs/FIRMWARE_PLAN.md`

## Safety and correctness

- Servo power must not be drawn from the ESP32 3.3 V pin.
- Share ground between logic and a non-isolated servo supply.
- Never connect 5 V sensor output directly to an ESP32 GPIO.
- Unknown or low-confidence predictions must not move the diverter.
- GPIOs and servo angles are placeholders, not confirmed wiring/calibration.
- A successful sort should only be reported after sensor feedback verifies it.
- The current classifier is a stub; this repository does not yet contain a trained waste-recognition model.
