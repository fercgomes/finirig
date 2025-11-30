#pragma once

#include <QWidget>
#include <QTimer>

namespace finirig::ui {

/**
 * @brief Audio level meter widget
 * 
 * Displays audio level with visual feedback (VU meter style)
 */
class LevelMeterWidget : public QWidget {
    Q_OBJECT

public:
    explicit LevelMeterWidget(const QString& label, QWidget* parent = nullptr);
    ~LevelMeterWidget() override = default;

    /**
     * @brief Update the level (0.0 to 1.0, or dB value)
     */
    void setLevel(float level);

    /**
     * @brief Set peak level
     */
    void setPeak(float peak);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString label_;
    float level_ = 0.0f;
    float peak_ = 0.0f;
    QTimer* peakHoldTimer_ = nullptr;
};

} // namespace finirig::ui

