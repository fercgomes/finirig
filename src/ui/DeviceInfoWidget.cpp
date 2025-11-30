#include "finirig/ui/DeviceInfoWidget.h"
#include "finirig/audio/AudioEngine.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QString>

namespace finirig::ui {

DeviceInfoWidget::DeviceInfoWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
}

void DeviceInfoWidget::setupUI() {
    auto* layout = new QVBoxLayout(this);
    
    // Device selection group
    auto* selectionGroup = new QGroupBox("Audio Device Selection", this);
    auto* selectionLayout = new QFormLayout(selectionGroup);
    
    inputDeviceCombo_ = new QComboBox(this);
    outputDeviceCombo_ = new QComboBox(this);
    
    selectionLayout->addRow("Input Device:", inputDeviceCombo_);
    selectionLayout->addRow("Output Device:", outputDeviceCombo_);
    
    connect(
        inputDeviceCombo_,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        &DeviceInfoWidget::onInputDeviceChanged
    );
    connect(
        outputDeviceCombo_,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        &DeviceInfoWidget::onOutputDeviceChanged
    );
    
    layout->addWidget(selectionGroup);
    
    // Device information group
    auto* infoGroup = new QGroupBox("Device Information", this);
    auto* infoLayout = new QFormLayout(infoGroup);
    
    deviceNameLabel_ = new QLabel("--", this);
    deviceTypeLabel_ = new QLabel("--", this);
    sampleRateLabel_ = new QLabel("--", this);
    bufferSizeLabel_ = new QLabel("--", this);
    inputChannelsLabel_ = new QLabel("--", this);
    outputChannelsLabel_ = new QLabel("--", this);
    
    infoLayout->addRow("Device Name:", deviceNameLabel_);
    infoLayout->addRow("Device Type:", deviceTypeLabel_);
    infoLayout->addRow("Sample Rate:", sampleRateLabel_);
    infoLayout->addRow("Buffer Size:", bufferSizeLabel_);
    infoLayout->addRow("Input Channels:", inputChannelsLabel_);
    infoLayout->addRow("Output Channels:", outputChannelsLabel_);
    
    layout->addWidget(infoGroup);
    layout->addStretch();
}

void DeviceInfoWidget::setAudioEngine(finirig::audio::AudioEngine* engine) {
    audioEngine_ = engine;
    refreshDevices();
    updateDeviceInfo();
}

void DeviceInfoWidget::refreshDevices() {
    if (!audioEngine_) {
        return;
    }
    
    // Store current selections
    QString currentInput = inputDeviceCombo_->currentText();
    QString currentOutput = outputDeviceCombo_->currentText();
    
    // Clear and populate input devices
    inputDeviceCombo_->clear();
    auto inputDevices = audioEngine_->getInputDeviceNames();
    for (const auto& device : inputDevices) {
        inputDeviceCombo_->addItem(QString::fromStdString(device.toStdString()));
    }
    
    // Restore selection if still available
    int inputIndex = inputDeviceCombo_->findText(currentInput);
    if (inputIndex >= 0) {
        inputDeviceCombo_->setCurrentIndex(inputIndex);
    }
    
    // Clear and populate output devices
    outputDeviceCombo_->clear();
    auto outputDevices = audioEngine_->getOutputDeviceNames();
    for (const auto& device : outputDevices) {
        outputDeviceCombo_->addItem(QString::fromStdString(device.toStdString()));
    }
    
    // Restore selection if still available
    int outputIndex = outputDeviceCombo_->findText(currentOutput);
    if (outputIndex >= 0) {
        outputDeviceCombo_->setCurrentIndex(outputIndex);
    }
}

void DeviceInfoWidget::updateDeviceInfo() {
    if (!audioEngine_) {
        return;
    }
    
    auto info = audioEngine_->getDeviceInfo();
    QString infoStr = QString::fromStdString(info.toStdString());
    
    // Parse info string (simple parsing)
    QStringList lines = infoStr.split("\n");
    for (const QString& line : lines) {
        if (line.startsWith("Device:")) {
            deviceNameLabel_->setText(line.mid(8).trimmed());
        } else if (line.startsWith("Type:")) {
            deviceTypeLabel_->setText(line.mid(6).trimmed());
        } else if (line.startsWith("Sample Rate:")) {
            sampleRateLabel_->setText(line.mid(13).trimmed());
        } else if (line.startsWith("Buffer Size:")) {
            bufferSizeLabel_->setText(line.mid(13).trimmed());
        } else if (line.startsWith("Input Channels:")) {
            inputChannelsLabel_->setText(line.mid(16).trimmed());
        } else if (line.startsWith("Output Channels:")) {
            outputChannelsLabel_->setText(line.mid(17).trimmed());
        }
    }
}

void DeviceInfoWidget::onInputDeviceChanged(int index) {
    if (!audioEngine_ || index < 0) {
        return;
    }
    
    QString deviceName = inputDeviceCombo_->itemText(index);
    audioEngine_->setInputDevice(deviceName.toStdString().c_str());
    updateDeviceInfo();
    emit inputDeviceChanged(deviceName);
}

void DeviceInfoWidget::onOutputDeviceChanged(int index) {
    if (!audioEngine_ || index < 0) {
        return;
    }
    
    QString deviceName = outputDeviceCombo_->itemText(index);
    audioEngine_->setOutputDevice(deviceName.toStdString().c_str());
    updateDeviceInfo();
    emit outputDeviceChanged(deviceName);
}

} // namespace finirig::ui

