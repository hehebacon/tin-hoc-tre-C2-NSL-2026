# Test and build commands

## Dataset layout tests (Python standard library only)

From the repository root:

    python3 -m unittest discover -s tests -v
    python3 tools/check_dataset.py datasets/waste

The dataset checker exits with code 1 when expected folders or images are missing. It checks layout and file extensions only.

## Firmware build

Install ESP-IDF 5.4.x and its toolchain, then run:

    idf.py set-target esp32s3
    idf.py build

GitHub Actions attempts the firmware build on pushes and pull requests. Inspect its run status and logs before considering the build verified.

## Hardware smoke-test order
1. Compile before connecting peripherals.
2. Test the sensor input with servo linkage disconnected.
3. Test servo PWM with suitable external supply and no mechanical load.
4. Calibrate home position and mechanical endpoints.
5. Bring up camera alone after configuring the exact pin map.
6. Integrate model inference only after preprocessing and output semantics are fixed.
7. Test the complete sorter with low-confidence and failure cases.
