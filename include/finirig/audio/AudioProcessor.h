#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace finirig::audio {

/**
 * @brief Base class for audio processing units
 * 
 * Provides interface for real-time audio processing.
 * All implementations must be real-time safe.
 */
class AudioProcessor {
public:
    virtual ~AudioProcessor() = default;

    /**
     * @brief Process a single audio sample
     * @param input Input sample value
     * @return Processed output sample
     */
    [[nodiscard]] virtual float processSample(float input) noexcept = 0;

    /**
     * @brief Process an audio buffer
     * @param buffer Audio buffer to process (in-place)
     * @param numChannels Number of channels
     * @param numSamples Number of samples per channel
     */
    virtual void processBlock(
        float* buffer,
        int numChannels,
        int numSamples
    ) noexcept;

    /**
     * @brief Prepare processor for new sample rate
     * @param sampleRate New sample rate
     */
    virtual void prepare(double sampleRate) {}

    /**
     * @brief Reset processor state
     */
    virtual void reset() {}
};

} // namespace finirig::audio

