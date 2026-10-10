#pragma once
#include "esp_err.h"
#include "app_state.h"
class ActuatorManager {
public:
    esp_err_t begin();
    esp_err_t moveToClass(WasteClass label);
    esp_err_t moveHome();
private:
    esp_err_t moveToAngle(int angleDeg);
    bool initialized_ = false;
};
