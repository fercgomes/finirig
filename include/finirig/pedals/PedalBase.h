#pragma once

#include "finirig/audio/AudioProcessor.h"

namespace finirig::pedals {

/**
 * @brief Base class for all guitar pedals
 * 
 * Provides common interface and functionality for pedal effects.
 * All pedals process audio sample-by-sample for maximum flexibility.
 */
class PedalBase : public finirig::audio::AudioProcessor {
public:
    PedalBase() = default;
    ~PedalBase() override = default;

    /**
     * @brief Enable or disable the pedal
     */
    void setEnabled(bool enabled) noexcept { enabled_ = enabled; }

    /**
     * @brief Check if pedal is enabled
     */
    [[nodiscard]] bool isEnabled() const noexcept { return enabled_; }

    /**
     * @brief Process sample through pedal (when enabled)
     */
    [[nodiscard]] float processSample(float input) noexcept override final;

protected:
    /**
     * @brief Process sample through pedal effect (implemented by subclasses)
     */
    [[nodiscard]] virtual float processSampleImpl(float input) noexcept = 0;

private:
    bool enabled_ = true;
};

} // namespace finirig::pedals

