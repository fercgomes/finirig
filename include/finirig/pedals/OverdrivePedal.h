#pragma once

#include "finirig/pedals/PedalBase.h"

namespace finirig::pedals {

/**
 * @brief Overdrive pedal effect
 * 
 * Classic overdrive effect with gain and tone controls.
 * Uses soft clipping for smooth distortion.
 */
class OverdrivePedal : public PedalBase {
public:
    OverdrivePedal();
    ~OverdrivePedal() override = default;

    /**
     * @brief Set drive amount (0.0 to 1.0)
     */
    void setDrive(float drive) noexcept;

    /**
     * @brief Get current drive amount
     */
    [[nodiscard]] float getDrive() const noexcept { return drive_; }

    /**
     * @brief Set tone control (0.0 = dark, 1.0 = bright)
     */
    void setTone(float tone) noexcept;

    /**
     * @brief Get current tone setting
     */
    [[nodiscard]] float getTone() const noexcept { return tone_; }

    /**
     * @brief Set output level (0.0 to 1.0)
     */
    void setLevel(float level) noexcept;

    /**
     * @brief Get current output level
     */
    [[nodiscard]] float getLevel() const noexcept { return level_; }

    void prepare(double sampleRate) override;
    void reset() override;

protected:
    [[nodiscard]] float processSampleImpl(float input) noexcept override;

private:
    float drive_ = 0.5f;
    float tone_ = 0.5f;
    float level_ = 0.7f;
    
    // Tone filter coefficients
    double sampleRate_ = 44100.0;
    float lowpassCoeff_ = 0.0f;
    float highpassCoeff_ = 0.0f;
    float filterState_ = 0.0f;
    
    void updateFilterCoefficients();
    
    /**
     * @brief Soft clipping function (tanh approximation)
     */
    [[nodiscard]] static float softClip(float x) noexcept;
};

} // namespace finirig::pedals

