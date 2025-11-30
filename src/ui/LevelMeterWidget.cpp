#include "finirig/ui/LevelMeterWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <algorithm>
#include <cmath>

namespace finirig::ui {

LevelMeterWidget::LevelMeterWidget(const QString& label, QWidget* parent)
    : QWidget(parent)
    , label_(label)
{
    setMinimumSize(30, 200);
    setMaximumWidth(50);
    
    // Peak hold timer
    peakHoldTimer_ = new QTimer(this);
    peakHoldTimer_->setSingleShot(true);
    peakHoldTimer_->setInterval(1000); // 1 second peak hold
}

void LevelMeterWidget::setLevel(float level) {
    level_ = std::clamp(level, 0.0f, 1.0f);
    update(); // Trigger repaint
}

void LevelMeterWidget::setPeak(float peak) {
    peak_ = std::clamp(peak, 0.0f, 1.0f);
    update();
    
    // Reset peak after hold time
    if (peakHoldTimer_) {
        peakHoldTimer_->stop();
        peakHoldTimer_->start();
        connect(peakHoldTimer_, &QTimer::timeout, this, [this]() {
            peak_ = 0.0f;
            update();
        });
    }
}

void LevelMeterWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    const int width = this->width();
    const int height = this->height();
    const int margin = 5;
    const int meterWidth = width - 2 * margin;
    const int meterHeight = height - 40; // Leave space for label
    const int meterX = margin;
    const int meterY = margin;
    
    // Background
    painter.fillRect(meterX, meterY, meterWidth, meterHeight, QColor(30, 30, 30));
    
    // Calculate level bar height
    int levelHeight = static_cast<int>(meterHeight * level_);
    int peakY = meterY + meterHeight - static_cast<int>(meterHeight * peak_);
    
    // Color gradient: green -> yellow -> red
    QColor color;
    if (level_ < 0.7f) {
        // Green to yellow
        float t = level_ / 0.7f;
        color = QColor(
            static_cast<int>(0 + t * 255),
            static_cast<int>(255 - t * 100),
            0
        );
    } else {
        // Yellow to red
        float t = (level_ - 0.7f) / 0.3f;
        color = QColor(
            255,
            static_cast<int>(155 - t * 155),
            0
        );
    }
    
    // Draw level bar
    if (levelHeight > 0) {
        painter.fillRect(
            meterX,
            meterY + meterHeight - levelHeight,
            meterWidth,
            levelHeight,
            color
        );
    }
    
    // Draw peak indicator
    if (peak_ > 0.0f) {
        painter.setPen(QPen(QColor(255, 255, 255), 2));
        painter.drawLine(meterX, peakY, meterX + meterWidth, peakY);
    }
    
    // Draw scale marks
    painter.setPen(QPen(QColor(100, 100, 100), 1));
    for (int i = 0; i <= 10; ++i) {
        int y = meterY + (meterHeight * i / 10);
        painter.drawLine(meterX, y, meterX + 5, y);
    }
    
    // Draw label
    painter.setPen(QPen(QColor(200, 200, 200), 1));
    painter.drawText(0, height - 20, width, 20, Qt::AlignCenter, label_);
    
    // Draw level text
    QString levelText = QString::number(static_cast<int>(level_ * 100)) + "%";
    painter.drawText(0, height - 5, width, 15, Qt::AlignCenter, levelText);
}

} // namespace finirig::ui

