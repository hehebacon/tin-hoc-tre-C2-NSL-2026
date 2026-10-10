#pragma once
#include <stdint.h>

// All GPIOs below are placeholders until the exact board/camera is selected.
// Negative camera pins intentionally disable camera initialization.
namespace app_config {
constexpr int kObjectSensorGpio = 4; // Example only; assumes active-low digital sensor
constexpr int kServoGpio = 5;        // Example only; PWM signal, external servo supply

constexpr int kCameraXclkGpio = -1;
constexpr int kCameraSccbSdaGpio = -1;
constexpr int kCameraSccbSclGpio = -1;
constexpr int kCameraD0Gpio = -1;
constexpr int kCameraD1Gpio = -1;
constexpr int kCameraD2Gpio = -1;
constexpr int kCameraD3Gpio = -1;
constexpr int kCameraD4Gpio = -1;
constexpr int kCameraD5Gpio = -1;
constexpr int kCameraD6Gpio = -1;
constexpr int kCameraD7Gpio = -1;
constexpr int kCameraVsyncGpio = -1;
constexpr int kCameraHrefGpio = -1;
constexpr int kCameraPclkGpio = -1;
constexpr int kCameraPwdnGpio = -1;
constexpr int kCameraResetGpio = -1;

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
