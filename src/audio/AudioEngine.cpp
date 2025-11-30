#include "finirig/audio/AudioEngine.h"
#include "finirig/audio/AudioProcessor.h"
#include <juce_audio_devices/juce_audio_devices.h>
#include <algorithm>
#include <cmath>

namespace finirig::audio {

AudioEngine::AudioEngine() {
    // Initialize audio device manager with default settings
    // Request at least 1 input channel for guitar input, 2 output channels for stereo
    deviceManager_.initialiseWithDefaultDevices(1, 2);
}

AudioEngine::~AudioEngine() {
    stop();
    deviceManager_.removeAudioCallback(this);
    deviceManager_.closeAudioDevice();
}

bool AudioEngine::initialize(double sampleRate, int bufferSize) {
    sampleRate_ = sampleRate;
    bufferSize_ = bufferSize;

    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager_.getAudioDeviceSetup(setup);
    setup.sampleRate = sampleRate;
    setup.bufferSize = bufferSize;
    
    // Ensure we have at least 1 input channel
    if (setup.inputChannels.countNumberOfSetBits() == 0) {
        setup.inputChannels.setBit(0, true); // Enable first input channel
    }
    
    // Ensure we have at least 1 output channel
    if (setup.outputChannels.countNumberOfSetBits() == 0) {
        setup.outputChannels.setBit(0, true); // Enable first output channel
        if (setup.outputChannels.getHighestBit() >= 1) {
            setup.outputChannels.setBit(1, true); // Enable second output channel for stereo
        }
    }

    auto error = deviceManager_.setAudioDeviceSetup(setup, true);
    if (error.isNotEmpty()) {
        return false;
    }

    return true;
}

bool AudioEngine::start() {
    if (isRunning_) {
        return true;
    }

    deviceManager_.addAudioCallback(this);
    isRunning_ = true;
    return true;
}

void AudioEngine::stop() {
    if (!isRunning_) {
        return;
    }

    deviceManager_.removeAudioCallback(this);
    isRunning_ = false;
}

void AudioEngine::setProcessor(std::unique_ptr<AudioProcessor> processor) {
    processor_ = std::move(processor);
    if (processor_) {
        processor_->prepare(sampleRate_);
    }
}

juce::StringArray AudioEngine::getInputDeviceNames() {
    juce::StringArray names;
    auto& deviceTypes = deviceManager_.getAvailableDeviceTypes();
    for (auto* type : deviceTypes) {
        if (type) {
            type->scanForDevices();
            auto devices = type->getDeviceNames(true); // true = input devices
            for (const auto& device : devices) {
                names.add(type->getTypeName() + ": " + device);
            }
        }
    }
    return names;
}

juce::StringArray AudioEngine::getOutputDeviceNames() {
    juce::StringArray names;
    auto& deviceTypes = deviceManager_.getAvailableDeviceTypes();
    for (auto* type : deviceTypes) {
        if (type) {
            type->scanForDevices();
            auto devices = type->getDeviceNames(false); // false = output devices
            for (const auto& device : devices) {
                names.add(type->getTypeName() + ": " + device);
            }
        }
    }
    return names;
}

juce::String AudioEngine::getCurrentInputDeviceName() const {
    auto* device = deviceManager_.getCurrentAudioDevice();
    if (device) {
        return device->getName();
    }
    return {};
}

juce::String AudioEngine::getCurrentOutputDeviceName() const {
    auto* device = deviceManager_.getCurrentAudioDevice();
    if (device) {
        return device->getName();
    }
    return {};
}

bool AudioEngine::setInputDevice(const juce::String& deviceName) {
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager_.getAudioDeviceSetup(setup);
    
    // Extract device name (remove type prefix if present)
    juce::String name = deviceName;
    int colonPos = name.indexOfChar(':');
    if (colonPos > 0) {
        name = name.substring(colonPos + 1).trim();
    }
    
    setup.inputDeviceName = name;
    
    auto error = deviceManager_.setAudioDeviceSetup(setup, true);
    if (error.isEmpty()) {
        // Update sample rate and buffer size after device change
        auto* device = deviceManager_.getCurrentAudioDevice();
        if (device) {
            sampleRate_ = device->getCurrentSampleRate();
            bufferSize_ = device->getCurrentBufferSizeSamples();
        }
    }
    return error.isEmpty();
}

bool AudioEngine::setOutputDevice(const juce::String& deviceName) {
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager_.getAudioDeviceSetup(setup);
    
    // Extract device name (remove type prefix if present)
    juce::String name = deviceName;
    int colonPos = name.indexOfChar(':');
    if (colonPos > 0) {
        name = name.substring(colonPos + 1).trim();
    }
    
    setup.outputDeviceName = name;
    
    auto error = deviceManager_.setAudioDeviceSetup(setup, true);
    if (error.isEmpty()) {
        // Update sample rate and buffer size after device change
        auto* device = deviceManager_.getCurrentAudioDevice();
        if (device) {
            sampleRate_ = device->getCurrentSampleRate();
            bufferSize_ = device->getCurrentBufferSizeSamples();
        }
    }
    return error.isEmpty();
}

juce::String AudioEngine::getDeviceInfo() const {
    auto* device = deviceManager_.getCurrentAudioDevice();
    if (!device) {
        return "No device";
    }
    
    juce::String info;
    info << "Device: " << device->getName() << "\n";
    info << "Type: " << device->getTypeName() << "\n";
    info << "Sample Rate: " << device->getCurrentSampleRate() << " Hz\n";
    info << "Buffer Size: " << device->getCurrentBufferSizeSamples() << " samples\n";
    info << "Input Channels: " << device->getInputChannelNames().size() << "\n";
    info << "Output Channels: " << device->getOutputChannelNames().size();
    
    return info;
}

void AudioEngine::updateLevels(
    const float* const* inputChannelData,
    int numInputChannels,
    const float* const* outputChannelData,
    int numOutputChannels,
    int numSamples
) noexcept {
    // Calculate peak levels (real-time safe)
    float inputPeak = 0.0f;
    if (numInputChannels > 0 && inputChannelData[0] != nullptr) {
        for (int sample = 0; sample < numSamples; ++sample) {
            float absValue = std::abs(inputChannelData[0][sample]);
            inputPeak = std::max(inputPeak, absValue);
        }
    }
    
    float outputPeak = 0.0f;
    if (numOutputChannels > 0 && outputChannelData[0] != nullptr) {
        for (int sample = 0; sample < numSamples; ++sample) {
            float absValue = std::abs(outputChannelData[0][sample]);
            outputPeak = std::max(outputPeak, absValue);
        }
    }
    
    // Update atomically (safe for UI thread to read)
    inputLevel_.store(inputPeak, std::memory_order_relaxed);
    outputLevel_.store(outputPeak, std::memory_order_relaxed);
}

void AudioEngine::audioDeviceIOCallbackWithContext(
    const float* const* inputChannelData,
    int numInputChannels,
    float* const* outputChannelData,
    int numOutputChannels,
    int numSamples,
    const juce::AudioIODeviceCallbackContext& context
) {
    (void)context; // Context not used in this implementation
    
    // Update levels first (real-time safe)
    updateLevels(inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples);
    
    // Clear output buffers
    for (int channel = 0; channel < numOutputChannels; ++channel) {
        if (outputChannelData[channel] != nullptr) {
            juce::FloatVectorOperations::clear(
                outputChannelData[channel],
                numSamples
            );
        }
    }

    // Process input if available
    // Note: Check both numInputChannels and that we actually have data
    if (numInputChannels > 0 && inputChannelData != nullptr && inputChannelData[0] != nullptr) {
        // Process mono input to stereo output (or mono to mono)
        const float* input = inputChannelData[0];
        float* output = outputChannelData[0];
        
        if (processor_) {
            // Process through audio processor
            for (int sample = 0; sample < numSamples; ++sample) {
                output[sample] = processor_->processSample(input[sample]);
            }
            
            // Copy to second channel if stereo output
            if (numOutputChannels > 1 && outputChannelData[1] != nullptr) {
                juce::FloatVectorOperations::copy(
                    outputChannelData[1],
                    output,
                    numSamples
                );
            }
        } else {
            // No processor - pass through
            juce::FloatVectorOperations::copy(output, input, numSamples);
            if (numOutputChannels > 1 && outputChannelData[1] != nullptr) {
                juce::FloatVectorOperations::copy(
                    outputChannelData[1],
                    input,
                    numSamples
                );
            }
        }
    } else {
        // No input - output silence (already cleared above)
    }
}

void AudioEngine::audioDeviceAboutToStart(juce::AudioIODevice* device) {
    if (device) {
        sampleRate_ = device->getCurrentSampleRate();
        bufferSize_ = device->getCurrentBufferSizeSamples();
        
        if (processor_) {
            processor_->prepare(sampleRate_);
        }
    }
}

void AudioEngine::audioDeviceStopped() {
    if (processor_) {
        processor_->reset();
    }
}

void AudioEngine::audioDeviceError(const juce::String& errorMessage) {
    // In a real application, you'd want to notify the UI thread
    // For now, we'll just ignore (or log if logging is set up)
    (void)errorMessage; // Suppress unused parameter warning
}

} // namespace finirig::audio

