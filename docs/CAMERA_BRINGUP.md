# Camera bring-up checklist

The camera driver is declared as an ESP-IDF component, and CameraManager uses the esp32-camera API. The camera GPIOs are deliberately set to -1 in main/app_config.h, so initialization is blocked until the exact board and camera pin map are known.

## Required before enabling camera

1. Record the exact ESP32-S3 board and camera module SKU.
2. Obtain the manufacturer's pin map; do not guess camera GPIOs.
3. Check camera signals against flash/PSRAM and all other peripherals.
4. Confirm PSRAM is enabled and available in the board's ESP-IDF configuration.
5. Set the camera GPIO constants in main/app_config.h.
6. Build and run a camera-only capture test before adding image decoding or inference.
7. Check frame buffer format and memory pressure with the selected frame size.

## Current frame settings

The manager requests QVGA JPEG, one frame buffer, and PSRAM frame-buffer placement. This is a starting point only. Model inference will need JPEG decode and resize/normalization into the model's expected tensor format. The current code does not yet perform that preprocessing or inference.
