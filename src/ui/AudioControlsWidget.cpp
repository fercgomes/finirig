#include "finirig/ui/AudioControlsWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>

namespace finirig::ui {

AudioControlsWidget::AudioControlsWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
}

void AudioControlsWidget::setupUI() {
    auto* layout = new QVBoxLayout(this);

    // Start/Stop button
    startStopButton_ = new QPushButton("Start Audio", this);
    connect(
        startStopButton_,
        &QPushButton::clicked,
        this,
        &AudioControlsWidget::onStartStopClicked
    );
    layout->addWidget(startStopButton_);

    // Status label
    statusLabel_ = new QLabel("Status: Stopped", this);
    layout->addWidget(statusLabel_);

    // Sample rate label
    sampleRateLabel_ = new QLabel("Sample Rate: --", this);
    layout->addWidget(sampleRateLabel_);

    // Buffer size label
    bufferSizeLabel_ = new QLabel("Buffer Size: --", this);
    layout->addWidget(bufferSizeLabel_);

    layout->addStretch();
}

void AudioControlsWidget::setAudioRunning(bool running) {
    isRunning_ = running;
    if (running) {
        startStopButton_->setText("Stop Audio");
        statusLabel_->setText("Status: Running");
    } else {
        startStopButton_->setText("Start Audio");
        statusLabel_->setText("Status: Stopped");
    }
}

void AudioControlsWidget::setSampleRate(double sampleRate) {
    sampleRateLabel_->setText(
        QString("Sample Rate: %1 Hz").arg(sampleRate, 0, 'f', 0)
    );
}

void AudioControlsWidget::setBufferSize(int bufferSize) {
    bufferSizeLabel_->setText(
        QString("Buffer Size: %1 samples").arg(bufferSize)
    );
}

void AudioControlsWidget::onStartStopClicked() {
    if (isRunning_) {
        emit stopAudioRequested();
    } else {
        emit startAudioRequested();
    }
}

} // namespace finirig::ui

