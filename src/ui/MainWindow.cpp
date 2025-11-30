#include "finirig/ui/MainWindow.h"
#include "finirig/ui/AudioControlsWidget.h"
#include "finirig/ui/LevelMeterWidget.h"
#include "finirig/ui/DeviceInfoWidget.h"
#include "finirig/audio/AudioEngine.h"
#include "finirig/pedals/OverdrivePedal.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QGroupBox>
#include <QApplication>
#include <QDir>

namespace finirig::ui {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , audioEngine_(std::make_unique<finirig::audio::AudioEngine>())
{
    setupUI();
    setupAudioEngine();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    setWindowTitle("Finirig - Guitar Processing");
    resize(1000, 700);

    // Set window icon and load icon for display
    QPixmap iconPixmap;
    
    // Try multiple paths to find the icon
    // On macOS, app is in .app/Contents/MacOS/, so we need to go up to find resources
    QString appDir = QApplication::applicationDirPath();
    QStringList iconPaths = {
        appDir + "/../Resources/icon.png",           // macOS bundle Resources folder (primary)
        appDir + "/../../resources/icon.png",        // macOS bundle, go up to project root
        appDir + "/../../../resources/icon.png",      // macOS bundle, alternative path
        QDir::currentPath() + "/resources/icon.png", // Absolute from current working dir
        "resources/icon.png",                        // Current directory
        "../resources/icon.png",                     // One level up
        "../../resources/icon.png",                 // Two levels up
    };
    
    for (const QString& path : iconPaths) {
        iconPixmap = QPixmap(path);
        if (!iconPixmap.isNull()) {
            qDebug() << "Loaded icon from:" << path;
            break;
        }
    }
    
    if (iconPixmap.isNull()) {
        qDebug() << "Warning: Could not load icon.png. Tried paths:" << iconPaths;
    }
    
    if (!iconPixmap.isNull()) {
        setWindowIcon(QIcon(iconPixmap));
    }

    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto* mainLayout = new QVBoxLayout(centralWidget);

    // Header with icon and title - make it prominent
    auto* headerWidget = new QWidget(this);
    headerWidget->setStyleSheet(
        "QWidget { "
        "background-color: #1e1e1e; "
        "border-bottom: 2px solid #3a3a3a; "
        "padding: 15px; "
        "}"
    );
    auto* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(20, 15, 20, 15);
    headerLayout->setSpacing(20);
    
    iconLabel_ = new QLabel(this);
    if (!iconPixmap.isNull()) {
        // Scale icon to a prominent size for the header
        QPixmap scaledIcon = iconPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        iconLabel_->setPixmap(scaledIcon);
        iconLabel_->setStyleSheet(
            "QLabel { "
            "background-color: transparent; "
            "border: 2px solid #4a4a4a; "
            "border-radius: 8px; "
            "padding: 5px; "
            "}"
        );
        iconLabel_->setFixedSize(110, 110);
    } else {
        // Fallback: show text if icon not found
        iconLabel_->setText("🎸");
        iconLabel_->setStyleSheet(
            "QLabel { "
            "font-size: 72px; "
            "background-color: transparent; "
            "border: 2px solid #4a4a4a; "
            "border-radius: 8px; "
            "padding: 5px; "
            "}"
        );
        iconLabel_->setFixedSize(110, 110);
        iconLabel_->setAlignment(Qt::AlignCenter);
    }
    iconLabel_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    headerLayout->addWidget(iconLabel_);
    
    auto* titleLabel = new QLabel(
        "<h1 style='color: #ffffff; margin: 0; font-size: 32px; font-weight: bold;'>Finirig</h1>"
        "<p style='color: #aaaaaa; margin: 5px 0 0 0; font-size: 14px;'>Guitar Processing Plugin</p>", 
        this
    );
    titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    titleLabel->setStyleSheet("QLabel { background-color: transparent; }");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    
    mainLayout->addWidget(headerWidget);

    // Main content area
    auto* contentLayout = new QHBoxLayout();

    // Left side: Controls and device info
    auto* leftLayout = new QVBoxLayout();
    
    audioControlsWidget_ = new AudioControlsWidget(this);
    leftLayout->addWidget(audioControlsWidget_);

    deviceInfoWidget_ = new DeviceInfoWidget(this);
    leftLayout->addWidget(deviceInfoWidget_);
    
    leftLayout->addStretch();
    contentLayout->addLayout(leftLayout, 2);

    // Right side: Level meters
    auto* metersGroup = new QGroupBox("Audio Levels", this);
    auto* metersLayout = new QHBoxLayout(metersGroup);
    metersLayout->setAlignment(Qt::AlignCenter);
    
    inputLevelMeter_ = new LevelMeterWidget("Input", this);
    outputLevelMeter_ = new LevelMeterWidget("Output", this);
    
    metersLayout->addWidget(inputLevelMeter_);
    metersLayout->addWidget(outputLevelMeter_);
    
    contentLayout->addWidget(metersGroup, 1);
    
    mainLayout->addLayout(contentLayout);

    // Connect signals
    connect(
        audioControlsWidget_,
        &AudioControlsWidget::startAudioRequested,
        this,
        &MainWindow::onStartAudio
    );
    connect(
        audioControlsWidget_,
        &AudioControlsWidget::stopAudioRequested,
        this,
        &MainWindow::onStopAudio
    );

    // Level meter update timer (30 FPS)
    levelUpdateTimer_ = new QTimer(this);
    connect(levelUpdateTimer_, &QTimer::timeout, this, &MainWindow::updateLevelMeters);
    levelUpdateTimer_->start(33); // ~30 FPS
}

void MainWindow::setupAudioEngine() {
    // Initialize with default settings
    constexpr double sampleRate = 44100.0;
    constexpr int bufferSize = 512;

    if (!audioEngine_->initialize(sampleRate, bufferSize)) {
        QMessageBox::warning(
            this,
            "Audio Initialization Error",
            "Failed to initialize audio device. Please check your audio settings."
        );
        return;
    }

    // Create a simple processing chain with an overdrive pedal
    auto pedal = std::make_unique<finirig::pedals::OverdrivePedal>();
    pedal->prepare(sampleRate);
    
    // Set processor (in a real app, you'd have a chain manager)
    audioEngine_->setProcessor(std::move(pedal));

    // Update UI with current settings
    audioControlsWidget_->setSampleRate(sampleRate);
    audioControlsWidget_->setBufferSize(bufferSize);
    
    // Setup device info widget
    deviceInfoWidget_->setAudioEngine(audioEngine_.get());
    deviceInfoWidget_->refreshDevices();
    deviceInfoWidget_->updateDeviceInfo();
}

void MainWindow::onStartAudio() {
    if (audioEngine_->start()) {
        audioControlsWidget_->setAudioRunning(true);
    } else {
        QMessageBox::warning(
            this,
            "Audio Start Error",
            "Failed to start audio processing."
        );
    }
}

void MainWindow::onStopAudio() {
    audioEngine_->stop();
    audioControlsWidget_->setAudioRunning(false);
    
    // Reset level meters
    if (inputLevelMeter_) {
        inputLevelMeter_->setLevel(0.0f);
    }
    if (outputLevelMeter_) {
        outputLevelMeter_->setLevel(0.0f);
    }
}

void MainWindow::updateLevelMeters() {
    if (!audioEngine_ || !audioEngine_->getSampleRate()) {
        return;
    }
    
    float inputLevel = audioEngine_->getInputLevel();
    float outputLevel = audioEngine_->getOutputLevel();
    
    if (inputLevelMeter_) {
        inputLevelMeter_->setLevel(inputLevel);
        inputLevelMeter_->setPeak(inputLevel);
    }
    
    if (outputLevelMeter_) {
        outputLevelMeter_->setLevel(outputLevel);
        outputLevelMeter_->setPeak(outputLevel);
    }
}

} // namespace finirig::ui

