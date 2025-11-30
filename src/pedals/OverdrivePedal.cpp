#include "finirig/pedals/OverdrivePedal.h"
#include <cmath>
#include <algorithm>

namespace finirig::pedals {

OverdrivePedal::OverdrivePedal() {
    updateFilterCoefficients();
}

void OverdrivePedal::setDrive(float drive) noexcept {
    drive_ = std::clamp(drive, 0.0f, 1.0f);
}

void OverdrivePedal::setTone(float tone) noexcept {
    tone_ = std::clamp(tone, 0.0f, 1.0f);
    updateFilterCoefficients();
}

void OverdrivePedal::setLevel(float level) noexcept {
    level_ = std::clamp(level, 0.0f, 1.0f);
}

void OverdrivePedal::prepare(double sampleRate) {
    sampleRate_ = sampleRate;
    updateFilterCoefficients();
    reset();
}

void OverdrivePedal::reset() {
    filterState_ = 0.0f;
}

float OverdrivePedal::processSampleImpl(float input) noexcept {
    // Apply drive (gain before clipping)
    float driven = input * (1.0f + drive_ * 9.0f); // Drive range: 1x to 10x
    
    // Soft clipping
    float clipped = softClip(driven);
    
    // Tone control (simple high-pass/low-pass blend)
    // Lowpass filter (one-pole)
    filterState_ = filterState_ * lowpassCoeff_ + clipped * (1.0f - lowpassCoeff_);
    float lowpassed = filterState_;
    
    // Highpass (input - lowpassed)
    float highpassed = clipped - lowpassed;
    
    // Blend based on tone control
    float output = lowpassed * (1.0f - tone_) + highpassed * tone_;
    
    // Apply level
    return output * level_;
}

void OverdrivePedal::updateFilterCoefficients() {
    // Simple one-pole filter coefficients
    // Cutoff frequency varies with tone control
    constexpr float minFreq = 200.0f;
    constexpr float maxFreq = 5000.0f;
    float cutoffFreq = minFreq + tone_ * (maxFreq - minFreq);
    
    // Calculate filter coefficient
    float rc = 1.0f / (2.0f * static_cast<float>(M_PI) * cutoffFreq);
    float dt = 1.0f / static_cast<float>(sampleRate_);
    lowpassCoeff_ = dt / (rc + dt);
    highpassCoeff_ = rc / (rc + dt);
}

float OverdrivePedal::softClip(float x) noexcept {
    // Fast tanh approximation for soft clipping
    // Using Pade approximation: x * (27 + x*x) / (27 + 9*x*x)
    constexpr float threshold = 3.0f;
    
    if (std::abs(x) < threshold) {
        float x2 = x * x;
        return x * (27.0f + x2) / (27.0f + 9.0f * x2);
    } else {
        // Clamp for values beyond threshold
        return x > 0.0f ? 1.0f : -1.0f;
    }
}

} // namespace finirig::pedals

