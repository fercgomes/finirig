#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QSlider;
QT_END_NAMESPACE

namespace finirig::ui {

/**
 * @brief Widget for audio device controls
 * 
 * Provides UI for starting/stopping audio and displaying
 * current audio device status.
 */
class AudioControlsWidget : public QWidget {
    Q_OBJECT

public:
    explicit AudioControlsWidget(QWidget* parent = nullptr);
    ~AudioControlsWidget() override = default;

    void setAudioRunning(bool running);
    void setSampleRate(double sampleRate);
    void setBufferSize(int bufferSize);

signals:
    void startAudioRequested();
    void stopAudioRequested();

private slots:
    void onStartStopClicked();

private:
    void setupUI();

    QPushButton* startStopButton_ = nullptr;
    QLabel* statusLabel_ = nullptr;
    QLabel* sampleRateLabel_ = nullptr;
    QLabel* bufferSizeLabel_ = nullptr;
    bool isRunning_ = false;
};

} // namespace finirig::ui

