#include "finirig/pedals/PedalBase.h"

namespace finirig::pedals {

float PedalBase::processSample(float input) noexcept {
    if (!enabled_) {
        return input;
    }
    return processSampleImpl(input);
}

} // namespace finirig::pedals

