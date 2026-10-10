#include "classifier.h"
bool Classifier::begin() {
    // TODO: initialize camera and selected on-device inference runtime.
    return true;
}
Classification Classifier::classify() {
    // Fail closed until a trained model is integrated. Never guess a class.
    return {WasteClass::UNKNOWN, 0.0f, false};
}
