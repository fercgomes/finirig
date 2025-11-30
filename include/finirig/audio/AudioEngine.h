#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <memory>

namespace finirig::audio {

class AudioProcessor;

/**
 * @brief Manages audio device I/O and processing pipeline
 * 
 * Real-time safe audio engine that handles device management
 * and routes audio through the processing chain.
 */
class AudioEngine : public juce::AudioIODeviceCallback {
public:
    explicit AudioEngine();
    ~AudioEngine() override;

    // Non-copyable
    AudioEngine(const AudioEngine&) = delete;
    AudioEngine& operator=(const AudioEngine&) = delete;

    /**
     * @brief Initialize audio engine with specified sample rate and buffer size
     * @param sampleRate Target sample rate (e.g., 44100, 48000)
     * @param bufferSize Buffer size in samples
     * @return true if initialization successful
     */
    bool initialize(double sampleRate, int bufferSize);

    /**
     * @brief Start audio processing
     * @return true if started successfully
     */
    bool start();

    /**
     * @brief Stop audio processing
     */
    void stop();

    /**
     * @brief Get current sample rate
     */
    [[nodiscard]] double getSampleRate() const noexcept { return sampleRate_; }

    /**
     * @brief Get current buffer size
     */
    [[nodiscard]] int getBufferSize() const noexcept { return bufferSize_; }

    /**
     * @brief Set the audio processor for the processing chain
     */
    void setProcessor(std::unique_ptr<AudioProcessor> processor);

    /**
     * @brief Get list of available input devices
     */
    [[nodiscard]] juce::StringArray getInputDeviceNames();

    /**
     * @brief Get list of available output devices
     */
    [[nodiscard]] juce::StringArray getOutputDeviceNames();

    /**
     * @brief Get current input device name
     */
    [[nodiscard]] juce::String getCurrentInputDeviceName() const;

    /**
     * @brief Get current output device name
     */
    [[nodiscard]] juce::String getCurrentOutputDeviceName() const;

    /**
     * @brief Set input device by name
     */
    bool setInputDevice(const juce::String& deviceName);

    /**
     * @brief Set output device by name
     */
    bool setOutputDevice(const juce::String& deviceName);

    /**
     * @brief Get current input level (peak, 0.0 to 1.0)
     */
    [[nodiscard]] float getInputLevel() const noexcept { return inputLevel_; }

    /**
     * @brief Get current output level (peak, 0.0 to 1.0)
     */
    [[nodiscard]] float getOutputLevel() const noexcept { return outputLevel_; }

    /**
     * @brief Get device information as string
     */
    [[nodiscard]] juce::String getDeviceInfo() const;

    // juce::AudioIODeviceCallback interface
    void audioDeviceIOCallbackWithContext(
        const float* const* inputChannelData,
        int numInputChannels,
        float* const* outputChannelData,
        int numOutputChannels,
        int numSamples,
        const juce::AudioIODeviceCallbackContext& context
    ) override;

    void audioDeviceAboutToStart(juce::AudioIODevice* device) override;
    void audioDeviceStopped() override;
    void audioDeviceError(const juce::String& errorMessage) override;

private:
    void updateLevels(
        const float* const* inputChannelData,
        int numInputChannels,
        const float* const* outputChannelData,
        int numOutputChannels,
        int numSamples
    ) noexcept;

    juce::AudioDeviceManager deviceManager_;
    std::unique_ptr<AudioProcessor> processor_;
    double sampleRate_ = 44100.0;
    int bufferSize_ = 512;
    bool isRunning_ = false;
    
    // Level monitoring (updated in audio callback, read from UI thread)
    mutable std::atomic<float> inputLevel_{0.0f};
    mutable std::atomic<float> outputLevel_{0.0f};
};

} // namespace finirig::audio

