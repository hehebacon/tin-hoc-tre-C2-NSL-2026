#include "control/sorter_controller.h"
#include "app_config.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
extern "C" void app_main(void) {
    static SorterController controller;
    if (!controller.begin()) {
        ESP_LOGE("c2_main", "Startup failed; system remains in FAULT");
        while (true) vTaskDelay(pdMS_TO_TICKS(1000));
    }
    while (true) {
        controller.tick();
        vTaskDelay(pdMS_TO_TICKS(app_config::kMainLoopDelayMs));
    }
}
