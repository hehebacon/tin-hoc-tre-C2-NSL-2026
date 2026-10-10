#pragma once
#include <stdint.h>

// PLACEHOLDER GPIOs. Confirm board and camera pin map before wiring.
namespace app_config {
constexpr int kObjectSensorGpio = 4; // Assumes active-low digital sensor
constexpr int kServoGpio = 5;        // PWM signal only; use separate servo supply
constexpr uint32_t kServoFrequencyHz = 50;
constexpr uint32_t kServoMinPulseUs = 500;
constexpr uint32_t kServoMaxPulseUs = 2500;
constexpr int kServoSafeAngleDeg = 90;
constexpr float kMinimumConfidence = 0.75f;
constexpr uint32_t kClassificationTimeoutMs = 5000;
constexpr uint32_t kActuationSettleMs = 700;
constexpr uint32_t kSensorDebounceMs = 60;
constexpr uint32_t kMainLoopDelayMs = 20;
}
