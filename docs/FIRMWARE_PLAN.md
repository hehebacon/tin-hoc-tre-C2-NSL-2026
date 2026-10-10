# Firmware milestones
1. Build the ESP-IDF project for ESP32-S3.
2. Test servo PWM with linkage removed and a correctly rated external supply.
3. Validate sensor active level, debounce and item removal.
4. Implement camera capture under controlled lighting.
5. Select and quantize an on-device classifier; measure flash, RAM, latency and unseen-image accuracy.
6. Map valid predictions to calibrated diverter positions.
7. Add exit/chute feedback and test blocked or repeated objects.
8. Test clean, dirty, crushed, partly hidden and visually similar waste items.

## First integrated prototype acceptance criteria
- Unknown or low-confidence predictions never move the diverter.
- Sensor chatter does not start multiple cycles.
- Missing or invalid model results fail closed.
- Servo and logic power are designed separately.
- A successful sort is only claimed when sensor feedback supports it.
- Classification works without network access.
