#pragma once

#include "finirig/audio/AudioProcessor.h"

namespace finirig::amps {

/**
 * @brief Base class for amplifier modeling
 * 
 * Provides interface for amplifier simulation including
 * preamp, power amp, and cabinet modeling.
 */
class AmpModel : public finirig::audio::AudioProcessor {
public:
    AmpModel() = default;
    ~AmpModel() override = default;

    /**
     * @brief Set gain (0.0 to 1.0)
     */
    virtual void setGain(float gain) noexcept = 0;

    /**
     * @brief Get current gain
     */
    [[nodiscard]] virtual float getGain() const noexcept = 0;

    /**
     * @brief Set master volume (0.0 to 1.0)
     */
    virtual void setMasterVolume(float volume) noexcept = 0;

    /**
     * @brief Get current master volume
     */
    [[nodiscard]] virtual float getMasterVolume() const noexcept = 0;
};

} // namespace finirig::amps

