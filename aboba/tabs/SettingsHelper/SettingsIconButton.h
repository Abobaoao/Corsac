#pragma once

#include <QToolButton>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QTimer>

class SettingsIconButton : public QToolButton {
    Q_OBJECT
public:
    SettingsIconButton(const QString& icon, 
                     const QString& tooltip,
                     QWidget* parent = nullptr);
    
    void animateHover(bool entering);
    void cancelHover();

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    
private:
    struct ButtonAnimationState {
        QPropertyAnimation* glowAnimation = nullptr;
        QPropertyAnimation* colorAnimation = nullptr;
    };
    
    void startHoverTimer();
    void performHoverAnimation(bool entering);
    
    QTimer* hoverTimer_ = nullptr;
    ButtonAnimationState animationState_;
    
    // Константы анимации
    static constexpr int kButtonHoverAnimationDuration = 200;
    static constexpr int kHoverDelay = 100;
    static constexpr int kButtonRadius = 20;
    static constexpr int kInputHeight = 40;
};