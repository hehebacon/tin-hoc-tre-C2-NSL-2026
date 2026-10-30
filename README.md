# C2 Smart Waste Sorter — ESP32-S3 Offline AI

Firmware scaffold for a camera-based waste sorter. The intended device captures an item, classifies it locally (without internet), and controls a servo diverter.

## Project direction

**ECO-SORT AI — Advanced demonstrator**
- Mechanical: gravity-fed chute with a servo-positioned selector.
- AI: offline inference on the selected edge device; no cloud inference.
- Scope: camera + dataset + trained model + confidence gate + physical sorting + sensor verification + local statistics/dashboard.

See:
- [System design](docs/SYSTEM_DESIGN.md) — overall architecture, chute concept, offline AI, firmware, electrical design and evaluation.
- [Implementation roadmap](docs/ROADMAP.md) — phases, team allocation, backlog, definition of done and risk register.

## Current status

- ESP-IDF project scaffold and fail-closed sorting state machine.
- ESP32 camera component dependency and a guarded camera capture interface.
- Dataset layout checker: `python3 tools/check_dataset.py`.
- AI classifier intentionally returns UNKNOWN until a real trained model is integrated.
- Camera GPIOs are unset (`-1`) until the exact board and camera module are selected.
- Not production-ready: compile, camera integration, AI inference and hardware tests remain.

## Architecture

Camera frame -> image preprocessing -> on-device classifier -> confidence gate -> sorter state machine -> servo chute selector -> exit verification -> local event counters.

## Proposed first prototype

- ESP32-S3 with PSRAM (N8R8-class board is a candidate, not yet purchased)
- Compatible camera module with a verified pin map
- Gravity-fed inspection chute and one servo-positioned selector
- Object-present sensor, exit sensor, and separate regulated servo supply
- Four provisional classes: paper, plastic, metal, other; uncertain items go to REVIEW/UNKNOWN

Confirm the official competition requirements before fixing labels or buying hardware. A camera-only model may confuse visually similar materials; measure this on held-out data and evaluate sensor fusion if necessary.

## Local checks

Run the Python dataset-layout tests with `python3 -m unittest discover -s tests -v`. GitHub Actions runs these checks and attempts an ESP-IDF build on pushes and pull requests.

## Build

Use ESP-IDF 5.x. From the project root:

```sh
idf.py set-target esp32s3
idf.py build
```

The component manager will fetch the camera dependency declared in `main/idf_component.yml`. This repository has not yet been built against a real ESP-IDF environment, so build success is not claimed.

Review these documents first:
- `docs/SYSTEM_DESIGN.md`
- `docs/ROADMAP.md`
- `docs/HARDWARE.md`
- `docs/CAMERA_BRINGUP.md`
- `main/perception/AI_MODEL_PIPELINE.md`
- `docs/FIRMWARE_PLAN.md`
- `docs/PROJECT_STATUS.md`
- `docs/DATASET_GUIDE.md`
- `docs/TESTING.md`

## Safety and correctness

- Servo power must not be drawn from the ESP32 3.3 V pin.
- Share ground between logic and a non-isolated servo supply.
- Never connect 5 V sensor output directly to an ESP32 GPIO.
- Unknown, invalid, or low-confidence predictions must not trigger normal sorting.
- GPIOs and servo angles are placeholders, not confirmed wiring/calibration.
- A successful sort should only be reported after exit-sensor feedback verifies passage.
- The classifier currently attempts camera initialization and frame capture, but still returns UNKNOWN because no trained waste-recognition model is integrated.
