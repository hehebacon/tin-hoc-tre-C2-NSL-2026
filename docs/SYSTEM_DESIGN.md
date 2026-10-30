# ECO-SORT AI — System Design

> Decision baseline: **gravity-fed chute**, **fully offline inference**, **Advanced demonstrator**.
> This document defines the intended system. It does not claim that hardware has been purchased, assembled, or validated.

## 1. Product goal

Build a tabletop smart waste sorter that:
1. detects one item entering the inspection area;
2. captures an image under repeatable lighting;
3. classifies it locally on the ESP32-S3;
4. routes it through a servo-driven chute only when the result is valid and sufficiently confident;
5. verifies item passage with sensors;
6. records local statistics and exposes a local dashboard without requiring internet.

The judging demonstration must show the complete physical loop, safe handling of uncertain predictions, measured model performance, and operation with network access disabled.

## 2. System architecture

```text
FEED SLOT
   |
   v
SINGULATION / ITEM-PRESENT SENSOR
   |
   v
FIXED-LIGHT INSPECTION CHAMBER <--- CAMERA
   |                                  |
   |                            JPEG FRAME
   |                                  v
   |                         DECODE / RESIZE / NORMALIZE
   |                                  |
   |                                  v
   |                         OFFLINE MODEL (ESP32-S3)
   |                                  |
   |                         LABEL + SCORE + VALIDITY
   |                                  v
   +--------------------------> CONFIDENCE / SAFETY GATE
                                      |
                         +------------+------------+
                         |                         |
                    VALID RESULT              UNKNOWN / FAULT
                         |                         |
                         v                         v
                 CHUTE POSITIONER             REVIEW / REJECT BIN
                         |
                         v
                  RELEASE ITEM
                         |
                         v
                 EXIT / BIN SENSOR
                         |
                         v
                LOCAL EVENT + COUNTERS
                         |
                         v
                LOCAL DASHBOARD (optional)
```

### Main subsystems

| Subsystem | Responsibility | Initial implementation |
|---|---|---|
| Feed and inspection | Present one item at a time in a repeatable camera position | Gravity-fed inlet, object sensor, enclosed lit area |
| Vision | Capture a usable image | Compatible camera; fixed light and background |
| Edge AI | Infer without cloud or internet | ESP32-S3 + PSRAM candidate; quantized model |
| Decision gate | Reject invalid or uncertain results | Explicit validity check and configurable confidence threshold |
| Chute | Route an item to the selected bin | Gravity chute with servo-positioned selector |
| Feedback | Verify that the item moved | Exit sensor; optional per-bin level/presence sensors |
| Telemetry | Count outcomes and faults | Local counters/log buffer; optional local Wi-Fi dashboard |

## 3. Mechanical concept: gravity-fed chute

### Proposed layout

- **Inlet:** a short feed opening that discourages two items entering together.
- **Inspection chamber:** a stable platform/short holding pocket where the item stops for imaging. The camera views the same region each cycle.
- **Lighting:** diffused white LEDs, matte background, and an enclosed hood to reduce changing ambient light and shadows.
- **Release gate:** a servo-operated gate holds the item during capture and inference.
- **Selector:** a positionable chute flap/rotating selector aligns the outlet with one of the collection paths.
- **Bins:** four removable, clearly labelled containers: PAPER, PLASTIC, METAL, OTHER. Add a separate REVIEW/UNKNOWN container if space permits; otherwise route uncertain items to OTHER but count them separately as REVIEW.
- **Feedback:** a sensor after the selector confirms passage. Bin sensors are an enhancement, not a prerequisite for the first integrated prototype.

### Operating sequence

1. Wait for a stable item-present signal.
2. Close/hold the release gate; wait for sensor debounce.
3. Capture an image and run preprocessing/inference.
4. Validate output and apply the confidence policy.
5. If valid, move the selector to the calibrated route and confirm the position if a position sensor is fitted.
6. Release the item; wait for exit feedback with a bounded timeout.
7. Return the selector and gate to HOME.
8. Log success only after exit feedback. On timeout, enter JAM/REVIEW and do not count a successful sort.

### Mechanical design constraints

- Keep the camera, light, and item position fixed relative to one another.
- Avoid narrow corners where soft paper or crushed packaging can snag.
- Keep the servo linkage outside the item path where possible.
- Make the inspection tray and bins removable for cleaning.
- Provide physical stops so the servo cannot drive the selector beyond its intended travel.
- Design for one item per cycle. Detect or manually clear multiple-item feeds rather than pretending the model can separate them.
- Do not finalize dimensions, servo torque, or angle values until the team measures representative items and builds a cardboard/mock-up chute.

### Important limitation

A camera-only classifier may confuse materials that look alike, especially metal-coated plastic, dark plastic, dirty paper, and reflective packaging. The team must measure this on its own held-out data. An inductive metal-presence sensor can be evaluated as an optional sensor-fusion input; it must not be treated as installed or accurate until tested.

## 4. Offline AI design

### Label policy

Initial labels:
- `PAPER`
- `PLASTIC`
- `METAL`
- `OTHER`
- `UNKNOWN` (system decision, not a normal training class)

Write a one-page annotation rule before collecting images. Define ambiguous composites (e.g. paper cups with plastic lining, foil packaging, mixed-material items) consistently. If the competition rubric requires different categories, update the label map before training.

### Data pipeline

1. Collect images from the actual camera or a camera with matching optics and lighting.
2. Use multiple physical examples per category, not many near-identical photos of one object.
3. Vary orientation, distance, lighting, cleanliness, crushing, and partial occlusion.
4. Label and review samples; remove duplicates and incorrect labels.
5. Split by physical object/source into train, validation, and held-out test sets. Do not place near-duplicate images in both train and test.
6. Train a compact image classifier on a PC.
7. Export to ONNX if required by the selected deployment path; quantize with the matching ESP-DL/ESP-PPQ toolchain if ESP-DL is selected.
8. Test the exported model on PC, then on the exact ESP32-S3 board.
9. Record model version, input shape, normalization, label order, threshold, model checksum, and evaluation report.

### Runtime contract

The firmware-facing inference result should contain:
- `valid`: false on camera, preprocessing, model, or output errors;
- `label`: one of the configured labels;
- `score`: finite score with documented semantics;
- `model_version`: identifier for the deployed model.

Do not assume raw logits are probabilities. Define the score conversion and confidence threshold using validation data. Keep the threshold configurable and record the selected value in the evaluation report.

### Safe decision policy

- Invalid frame, invalid model output, timeout, or non-finite score → REVIEW; no normal route actuation.
- Score below the validated threshold → REVIEW; no normal route actuation.
- Valid high-confidence prediction → route to its mapped bin.
- No sensor-confirmed exit → JAM/REVIEW; do not increment successful-sort count.
- Model missing at boot → firmware may run diagnostics, but it must not claim AI sorting is available.

### Runtime candidate

ESP32-S3 with PSRAM is the current candidate, not a confirmed purchase. ESP-DL is a candidate runtime, not a fixed dependency until a model has been trained and its operators, memory use, and latency checked against the official support matrix. A model file from another runtime is not automatically compatible with ESP-DL.

## 5. Firmware architecture

Keep modules separated so hardware and model choices can change without rewriting the state machine.

| Module | Interface / responsibility |
|---|---|
| `feed_sensor` | Debounced item-present and item-removed events |
| `camera_manager` | Camera init, bounded capture, frame metadata |
| `image_preprocess` | Decode, resize, normalize using versioned model settings |
| `classifier` | Inference result: valid, label, score, model version |
| `decision_gate` | Confidence policy and REVIEW decision |
| `chute_controller` | HOME and calibrated route positions; bounded movement |
| `exit_sensor` | Passage confirmation and timeout |
| `sorter_fsm` | Owns the complete cycle and fault transitions |
| `event_logger` | Local counts, faults, timing, and model version |
| `local_dashboard` | Optional local-only status/metrics interface |

### Suggested state flow

`IDLE → ITEM_DETECTED → CAPTURING → CLASSIFYING → ROUTING → RELEASING → VERIFYING → RETURN_HOME → IDLE`

Exceptional states:
- `REVIEW`: unknown/low-confidence result; wait for manual removal or a defined safe handling action.
- `JAM`: item did not pass before timeout.
- `FAULT`: camera, sensor, servo, or model failure.

All transitions must have bounded timeouts. Sensor chatter must not start multiple cycles. A reset must return outputs to a known safe state.

## 6. Electrical and power design

- Power the ESP32-S3 and camera according to the exact board manufacturer's specification.
- Power the servo from a separate regulated supply sized for its measured/stated stall current.
- Join grounds for a non-isolated servo PWM signal.
- Never power a servo from the ESP32 3.3 V rail.
- Never feed a 5 V sensor output directly into an ESP32 GPIO; verify the sensor's output circuit and logic level.
- Add suitable power decoupling and strain relief after the actual current draw and wiring are known.
- Keep camera GPIO, PWM, and sensor pins conflict-free using the exact board pinout.
- GPIO numbers and servo angles in existing scaffold files are placeholders until confirmed.

## 7. Local dashboard (Advanced enhancement)

The dashboard is not part of the AI decision path. It is a local monitoring layer and must never be required for sorting.

Suggested display:
- current state: IDLE / CLASSIFYING / ROUTING / REVIEW / JAM / FAULT;
- per-class successful-sort counters;
- REVIEW and JAM counters;
- last prediction, score, model version, and inference time;
- camera/sensor/servo health indicators;
- session reset/export action if time permits.

Keep the inference and sorting cycle functional with Wi-Fi disabled. If a local web page is added, use local network access only and do not upload images or telemetry to a cloud service.

## 8. Evaluation plan and measurable evidence

Report at least:
- held-out sample count and class distribution;
- confusion matrix;
- per-class precision, recall, and F1;
- accuracy, with class imbalance disclosed;
- inference latency (median and p95);
- peak RAM/PSRAM and firmware/model flash use;
- camera capture success rate over at least 100 consecutive trials;
- physical routing success rate per class over repeated trials;
- REVIEW rate and false-accept rate;
- jam/fault recovery results;
- offline test result with network disconnected.

Keep raw test logs and evaluation scripts in the repository. Do not claim a target percentage until measurements exist.

## 9. Design gates — decisions still open

Do not treat these as settled until verified:
- exact ESP32-S3 board and PSRAM capacity;
- camera module and verified pin map;
- exact servo model and supply;
- object/exit sensor models and logic levels;
- final class definitions required by the competition;
- model architecture, input resolution, runtime and confidence threshold;
- chute dimensions, route angles and physical bin layout.

A design gate is closed only when the choice is documented with a source, test, or measurement.
