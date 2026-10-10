#include "camera_manager.h"
#include "app_config.h"
#include "esp_log.h"
namespace { const char* TAG = "camera"; }
esp_err_t CameraManager::begin() {
    // Pin assignments are intentionally unset until the exact board/camera is selected.
    if (app_config::kCameraXclkGpio < 0 ||
        app_config::kCameraSccbSdaGpio < 0 || app_config::kCameraSccbSclGpio < 0 ||
        app_config::kCameraD0Gpio < 0 || app_config::kCameraD1Gpio < 0 ||
        app_config::kCameraD2Gpio < 0 || app_config::kCameraD3Gpio < 0 ||
        app_config::kCameraD4Gpio < 0 || app_config::kCameraD5Gpio < 0 ||
        app_config::kCameraD6Gpio < 0 || app_config::kCameraD7Gpio < 0 ||
        app_config::kCameraVsyncGpio < 0 || app_config::kCameraHrefGpio < 0 ||
        app_config::kCameraPclkGpio < 0) {
        ESP_LOGW(TAG, "Camera GPIO map is not configured; camera remains disabled");
        return ESP_ERR_INVALID_STATE;
    }
    camera_config_t config = {};
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = app_config::kCameraD0Gpio;
    config.pin_d1 = app_config::kCameraD1Gpio;
    config.pin_d2 = app_config::kCameraD2Gpio;
    config.pin_d3 = app_config::kCameraD3Gpio;
    config.pin_d4 = app_config::kCameraD4Gpio;
    config.pin_d5 = app_config::kCameraD5Gpio;
    config.pin_d6 = app_config::kCameraD6Gpio;
    config.pin_d7 = app_config::kCameraD7Gpio;
    config.pin_xclk = app_config::kCameraXclkGpio;
    config.pin_pclk = app_config::kCameraPclkGpio;
    config.pin_vsync = app_config::kCameraVsyncGpio;
    config.pin_href = app_config::kCameraHrefGpio;
    config.pin_sccb_sda = app_config::kCameraSccbSdaGpio;
    config.pin_sccb_scl = app_config::kCameraSccbSclGpio;
    config.pin_pwdn = app_config::kCameraPwdnGpio;
    config.pin_reset = app_config::kCameraResetGpio;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    const esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_camera_init failed: 0x%x", static_cast<unsigned>(err));
        return err;
    }
    ready_ = true;
    ESP_LOGI(TAG, "Camera initialized at QVGA JPEG; AI preprocessing remains separate");
    return ESP_OK;
}
camera_fb_t* CameraManager::capture() {
    return ready_ ? esp_camera_fb_get() : nullptr;
}
void CameraManager::release(camera_fb_t* frame) {
    if (frame != nullptr) esp_camera_fb_return(frame);
}
