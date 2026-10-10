#pragma once
#include <stdint.h>
enum class WasteClass : uint8_t { UNKNOWN = 0, PAPER, PLASTIC, METAL, OTHER, COUNT };
struct Classification {
    WasteClass label = WasteClass::UNKNOWN;
    float confidence = 0.0f;
    bool valid = false;
};
enum class SorterState : uint8_t { IDLE = 0, CLASSIFYING, SORTING, VERIFYING, NEEDS_REVIEW, FAULT };
inline const char* wasteClassName(WasteClass label) {
    switch (label) {
        case WasteClass::PAPER: return "paper";
        case WasteClass::PLASTIC: return "plastic";
        case WasteClass::METAL: return "metal";
        case WasteClass::OTHER: return "other";
        default: return "unknown";
    }
}
