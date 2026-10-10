#include "actuator_manager.h"
#include <algorithm>
#include "app_config.h"
#include "driver/ledc.h"
namespace {
constexpr ledc_mode_t kSpeedMode = LEDC_LOW_SPEED_MODE;
// Keep servo timer/channel distinct from the camera driver's LEDC timer/channel.
constexpr ledc_timer_t kTimer = LEDC_TIMER_1;
constexpr ledc_channel_t kChannel = LEDC_CHANNEL_1;
constexpr ledc_timer_bit_t kResolution = LEDC_TIMER_14_BIT;
constexpr uint32_t kDutyMax = (1U << 14U) - 1U;
constexpr uint32_t kPwmPeriodUs = 1000000U / app_config::kServoFrequencyHz;
uint32_t pulseUsToDuty(uint32_t pulseUs) {
    return static_cast<uint32_t>((static_cast<uint64_t>(pulseUs) * kDutyMax) / kPwmPeriodUs);
}
}
esp_err_t ActuatorManager::begin() {
    ledc_timer_config_t timer = {};
    timer.speed_mode = kSpeedMode;
    timer.duty_resolution = kResolution;
    timer.timer_num = kTimer;
    timer.freq_hz = app_config::kServoFrequencyHz;
    timer.clk_cfg = LEDC_AUTO_CLK;
    esp_err_t err = ledc_timer_config(&timer);
    if (err != ESP_OK) return err;
    ledc_channel_config_t channel = {};
    channel.gpio_num = app_config::kServoGpio;
    channel.speed_mode = kSpeedMode;
    channel.channel = kChannel;
    channel.intr_type = LEDC_INTR_DISABLE;
    channel.timer_sel = kTimer;
    channel.duty = pulseUsToDuty(1500);
    channel.hpoint = 0;
    err = ledc_channel_config(&channel);
    if (err != ESP_OK) return err;
    initialized_ = true;
    return moveHome();
}
esp_err_t ActuatorManager::moveToClass(WasteClass label) {
    // Provisional angles; calibrate with linkage removed before installation.
    switch (label) {
        case WasteClass::PAPER: return moveToAngle(35);
        case WasteClass::PLASTIC: return moveToAngle(75);
        case WasteClass::METAL: return moveToAngle(115);
        case WasteClass::OTHER: return moveToAngle(155);
        default: return ESP_ERR_INVALID_ARG;
    }
}
esp_err_t ActuatorManager::moveHome() {
    return moveToAngle(app_config::kServoSafeAngleDeg);
}
esp_err_t ActuatorManager::moveToAngle(int angleDeg) {
    if (!initialized_) return ESP_ERR_INVALID_STATE;
    angleDeg = std::max(0, std::min(180, angleDeg));
    const uint32_t pulseUs = app_config::kServoMinPulseUs +
        (static_cast<uint32_t>(angleDeg) * (app_config::kServoMaxPulseUs - app_config::kServoMinPulseUs)) / 180U;
    esp_err_t err = ledc_set_duty(kSpeedMode, kChannel, pulseUsToDuty(pulseUs));
    if (err != ESP_OK) return err;
    return ledc_update_duty(kSpeedMode, kChannel);
}
