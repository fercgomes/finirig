#include "finirig/audio/AudioProcessor.h"

namespace finirig::audio {

void AudioProcessor::processBlock(
    float* buffer,
    int numChannels,
    int numSamples
) noexcept {
    // Default implementation: process each sample
    for (int sample = 0; sample < numSamples; ++sample) {
        float input = buffer[sample * numChannels];
        float output = processSample(input);
        
        // Write to all channels (mono to stereo/mono)
        for (int channel = 0; channel < numChannels; ++channel) {
            buffer[sample * numChannels + channel] = output;
        }
    }
}

} // namespace finirig::audio

