#include "sorter_controller.h"
#include <cmath>
#include "app_config.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
namespace { const char* TAG = "sorter"; }
bool SorterController::begin() {
    gpio_config_t sensor = {};
    sensor.pin_bit_mask = 1ULL << app_config::kObjectSensorGpio;
    sensor.mode = GPIO_MODE_INPUT;
    sensor.pull_up_en = GPIO_PULLUP_ENABLE;
    sensor.pull_down_en = GPIO_PULLDOWN_DISABLE;
    sensor.intr_type = GPIO_INTR_DISABLE;
    if (gpio_config(&sensor) != ESP_OK) { setState(SorterState::FAULT); return false; }
    if (!classifier_.begin()) {
        ESP_LOGE(TAG, "Classifier initialization failed");
        setState(SorterState::FAULT); return false;
    }
    if (actuators_.begin() != ESP_OK) {
        ESP_LOGE(TAG, "Actuator initialization failed");
        setState(SorterState::FAULT); return false;
    }
    previousObjectPresent_ = objectPresent();
    lastObjectChangeUs_ = esp_timer_get_time();
    setState(SorterState::IDLE);
    ESP_LOGI(TAG, "Controller initialized; classifier is currently a safe stub");
    return true;
}
bool SorterController::objectPresent() const {
    // Assumes active-low sensor and pull-up. Adapt to the actual sensor.
    return gpio_get_level(static_cast<gpio_num_t>(app_config::kObjectSensorGpio)) == 0;
}
void SorterController::setState(SorterState next) {
    state_ = next;
    stateStartedUs_ = esp_timer_get_time();
    ESP_LOGI(TAG, "State -> %d", static_cast<int>(next));
}
void SorterController::tick() {
    if (state_ == SorterState::FAULT) return;
    const int64_t now = esp_timer_get_time();
    const bool present = objectPresent();
    if (present != previousObjectPresent_) {
        previousObjectPresent_ = present;
        lastObjectChangeUs_ = now;
    }
    const bool stable = (now - lastObjectChangeUs_) >=
        static_cast<int64_t>(app_config::kSensorDebounceMs) * 1000;
    switch (state_) {
        case SorterState::IDLE:
            if (stable && present && !objectHandled_) setState(SorterState::CLASSIFYING);
            if (stable && !present) objectHandled_ = false;
            break;
        case SorterState::CLASSIFYING: {
            if ((now - stateStartedUs_) > static_cast<int64_t>(app_config::kClassificationTimeoutMs) * 1000) {
                ESP_LOGW(TAG, "Classification timeout; requiring review");
                setState(SorterState::NEEDS_REVIEW); break;
            }
            const Classification result = classifier_.classify();
            if (!result.valid || result.label == WasteClass::UNKNOWN ||
                result.label >= WasteClass::COUNT || !std::isfinite(result.confidence) ||
                result.confidence < 0.0f || result.confidence > 1.0f ||
                result.confidence < app_config::kMinimumConfidence) {
                ESP_LOGW(TAG, "No reliable prediction; diverter will not move");
                setState(SorterState::NEEDS_REVIEW); break;
            }
            if (actuators_.moveToClass(result.label) != ESP_OK) {
                setState(SorterState::FAULT); break;
            }
            objectHandled_ = true;
            setState(SorterState::SORTING);
            break;
        }
        case SorterState::SORTING:
            if ((now - stateStartedUs_) >= static_cast<int64_t>(app_config::kActuationSettleMs) * 1000) {
                if (actuators_.moveHome() != ESP_OK) { setState(SorterState::FAULT); break; }
                setState(SorterState::VERIFYING);
            }
            break;
        case SorterState::VERIFYING:
            // TODO: add exit/chute sensor; without it successful sorting is unverified.
            if ((now - stateStartedUs_) >= 250000) setState(SorterState::IDLE);
            break;
        case SorterState::NEEDS_REVIEW:
            if (stable && !present) { objectHandled_ = false; setState(SorterState::IDLE); }
            break;
        case SorterState::FAULT: break;
    }
}
