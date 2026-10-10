# Offline AI model integration contract

## Runtime decision
The first target candidate is ESP32-S3 with PSRAM. Use ESP-DL or another runtime only after a small custom classifier has been trained and benchmarked. ESP-DL models require the Espressif-compatible quantization/export path; a random TensorFlow Lite int8 file is not automatically compatible.

## Input contract
- Capture one item in a fixed, lit inspection tray.
- Start from QVGA JPEG camera capture, then decode and resize to the model's exact input shape.
- Apply exactly the normalization used during training.
- Keep preprocessing and label order versioned alongside the model.
- The model must return a finite score for each configured class.

## Output contract
- label: PAPER, PLASTIC, METAL, OTHER, or UNKNOWN.
- confidence: score in [0, 1] according to the model's documented output semantics.
- valid: false if capture, preprocessing, inference or output validation fails.

Do not interpret arbitrary logits as probabilities. Define and test output conversion for the chosen model.

## Model acceptance gate
1. Record dataset source and class balance.
2. Keep a held-out test set not used during training.
3. Measure per-class precision/recall and confusion matrix.
4. Test mixed lighting, shadows, crushed/dirty items, partial occlusion and similar objects.
5. Measure inference latency, peak memory and firmware image size on the exact target.
6. Confirm UNKNOWN/low-confidence outputs leave the diverter unmoved.

## Current state
No trained waste model is included. classifier.cpp now attempts camera initialization and frame capture, but intentionally returns UNKNOWN until a real model, preprocessing and output mapping are integrated. Camera pin mapping is unset until the exact board is chosen, so startup fails safely until it is configured.
