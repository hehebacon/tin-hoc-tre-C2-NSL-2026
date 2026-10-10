#include "classifier.h"
#include "esp_log.h"
namespace { const char* TAG = "classifier"; }
bool Classifier::begin() {
    // Camera setup must succeed before this camera-based pipeline can start.
    // Camera GPIOs remain unset until the exact board/module is selected.
    const esp_err_t err = camera_.begin();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera unavailable; sorter startup must fail safely");
        return false;
    }
    ESP_LOGW(TAG, "Camera is ready, but no trained inference model is integrated yet");
    return true;
}
Classification Classifier::classify() {
    // Exercise the camera capture path, but never invent a class without a model.
    camera_fb_t* frame = camera_.capture();
    if (frame == nullptr) {
        ESP_LOGE(TAG, "Camera capture failed");
        return {WasteClass::UNKNOWN, 0.0f, false};
    }
    camera_.release(frame);
    return {WasteClass::UNKNOWN, 0.0f, false};
}
