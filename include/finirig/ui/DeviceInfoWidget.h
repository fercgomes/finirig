#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
QT_END_NAMESPACE

namespace finirig::audio {
class AudioEngine;
}

namespace finirig::ui {

/**
 * @brief Widget for audio device information and selection
 * 
 * Displays current audio device info and allows device selection
 */
class DeviceInfoWidget : public QWidget {
    Q_OBJECT

public:
    explicit DeviceInfoWidget(QWidget* parent = nullptr);
    ~DeviceInfoWidget() override = default;

    /**
     * @brief Set the audio engine to monitor
     */
    void setAudioEngine(finirig::audio::AudioEngine* engine);

    /**
     * @brief Refresh device lists
     */
    void refreshDevices();

    /**
     * @brief Update device information display
     */
    void updateDeviceInfo();

signals:
    void inputDeviceChanged(const QString& deviceName);
    void outputDeviceChanged(const QString& deviceName);

private slots:
    void onInputDeviceChanged(int index);
    void onOutputDeviceChanged(int index);

private:
    void setupUI();
    void populateDeviceLists();

    finirig::audio::AudioEngine* audioEngine_ = nullptr;
    
    QComboBox* inputDeviceCombo_ = nullptr;
    QComboBox* outputDeviceCombo_ = nullptr;
    QLabel* deviceNameLabel_ = nullptr;
    QLabel* deviceTypeLabel_ = nullptr;
    QLabel* sampleRateLabel_ = nullptr;
    QLabel* bufferSizeLabel_ = nullptr;
    QLabel* inputChannelsLabel_ = nullptr;
    QLabel* outputChannelsLabel_ = nullptr;
};

} // namespace finirig::ui

