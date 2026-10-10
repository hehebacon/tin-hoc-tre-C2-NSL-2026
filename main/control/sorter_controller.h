#pragma once
#include "app_state.h"
#include "actuators/actuator_manager.h"
#include "perception/classifier.h"
class SorterController {
public:
    bool begin();
    void tick();
    SorterState state() const { return state_; }
private:
    bool objectPresent() const;
    void setState(SorterState next);
    Classifier classifier_;
    ActuatorManager actuators_;
    SorterState state_ = SorterState::FAULT;
    int64_t stateStartedUs_ = 0;
    int64_t lastObjectChangeUs_ = 0;
    bool previousObjectPresent_ = false;
    bool objectHandled_ = false;
};
