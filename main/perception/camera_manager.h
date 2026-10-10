#pragma once
#include "esp_err.h"
#include "esp_camera.h"
class CameraManager {
public:
    esp_err_t begin();
    camera_fb_t* capture();
    void release(camera_fb_t* frame);
    bool ready() const { return ready_; }
private:
    bool ready_ = false;
};
