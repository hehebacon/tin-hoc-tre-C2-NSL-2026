# Hardware bring-up notes

## Proposed first prototype
- ESP32-S3 development board with PSRAM (N8R8 class is a candidate).
- Camera compatible with the chosen board and inference pipeline.
- One servo diverter, rated for the mechanical load.
- Object-present sensor with logic levels compatible with ESP32-S3.
- Optional end-stop or chute sensor to verify completed sorting.
- Separate regulated servo supply sized for the servo stall current.

## Before connecting power
1. Confirm the exact board and camera pin map.
2. Ensure camera signals do not conflict with the servo or object-sensor GPIO.
3. Confirm sensor output is 3.3 V compatible; never connect 5 V logic directly to ESP32 GPIO.
4. Power the servo from a suitable external supply, not the ESP32 3.3 V pin.
5. Share logic ground with the servo supply for a non-isolated PWM signal.
6. Remove the diverter linkage or disconnect servo power during the first firmware smoke test.
7. Calibrate home and bin angles before installing the linkage.

## Placeholder pin map
main/app_config.h currently uses GPIO 4 for an active-low object sensor and GPIO 5 for servo PWM. These are examples only, not confirmed wiring. Camera modules often use many GPIOs; verify the chosen board pin map.

## Provisional diverter angles
| Label | Angle |
|---|---:|
| Paper | 35 degrees |
| Plastic | 75 degrees |
| Metal | 115 degrees |
| Other | 155 degrees |
| Home | 90 degrees |

Calibrate mechanical endpoints carefully; do not allow the servo to jam against the frame.

## AI integration gate
The current classifier returns UNKNOWN intentionally. Integrate a quantized model only after labels and dataset are confirmed, camera capture works, memory and inference latency are measured, and low-confidence predictions are verified to leave the diverter unmoved.
