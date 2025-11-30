#pragma once

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QTimer;
QT_END_NAMESPACE

namespace finirig::audio {
class AudioEngine;
}

namespace finirig::ui {

class AudioControlsWidget;
class LevelMeterWidget;
class DeviceInfoWidget;

/**
 * @brief Main application window
 * 
 * Qt-based main window for the standalone application.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void onStartAudio();
    void onStopAudio();
    void updateLevelMeters();

private:
    void setupUI();
    void setupAudioEngine();

    std::unique_ptr<finirig::audio::AudioEngine> audioEngine_;
    AudioControlsWidget* audioControlsWidget_ = nullptr;
    LevelMeterWidget* inputLevelMeter_ = nullptr;
    LevelMeterWidget* outputLevelMeter_ = nullptr;
    DeviceInfoWidget* deviceInfoWidget_ = nullptr;
    QLabel* iconLabel_ = nullptr;
    QTimer* levelUpdateTimer_ = nullptr;
};

} // namespace finirig::ui

