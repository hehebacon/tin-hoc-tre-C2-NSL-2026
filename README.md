# C2 Smart Waste Sorter — ESP32-S3 Offline AI

Firmware scaffold for a camera-based waste sorter. The device is designed to classify an item locally and then move a servo-controlled diverter to the selected bin.

## Current status

- ESP-IDF project scaffold and safe sorting state machine.
- Hardware pins are placeholders and must be confirmed against the purchased board.
- The AI classifier is an interface/stub until a trained, quantized model is selected and integrated.
- Not production-ready: compile and hardware tests are still required.

## Initial architecture

Camera frame -> on-device classifier -> confidence gate -> sorter state machine -> servo diverter.

Suggested first prototype:
- ESP32-S3 with PSRAM (e.g. N8R8 class board)
- compatible camera module
- external servo power supply sized for the chosen servo
- object-present sensor and optional end-stop feedback
- 4 labels as a provisional configuration: paper, plastic, metal, other

Do not rely on the provisional labels until the project rules and training dataset are confirmed.

## Build

Use ESP-IDF 5.x. From the project root:

```sh
idf.py set-target esp32s3
idf.py build
```

Before connecting hardware, review `main/app_config.h` and `docs/HARDWARE.md`.

## Safety

- Servo power must not be drawn from the ESP32 3.3 V pin.
- Connect grounds together when using a separate servo supply and non-isolated logic signals.
- An unknown or low-confidence prediction must not trigger a bin movement.
- GPIO values in the scaffold are examples, not confirmed wiring.
