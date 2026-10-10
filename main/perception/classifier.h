#pragma once
#include "app_state.h"
#include "camera_manager.h"
class Classifier {
public:
    bool begin();
    Classification classify();
private:
    CameraManager camera_;
};
