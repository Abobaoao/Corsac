#include "SettingsIconButton.h"
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>

SettingsIconButton::SettingsIconButton(const QString& icon, 
                                     const QString& tooltip,
                                     QWidget* parent)
    : QToolButton(parent)
{
    setText(icon);
    setToolTip(tooltip);
    setStyleSheet(QString(R"(
        QToolButton {
            background: rgba(0,0,0,150);
            border: none;
            border-radius: %1px;
            font-size: 18px;
            color: white;
            width: %2px;
            height: %2px;
        }
        QToolButton:hover {
            background: rgba(50,50,50,200);
        }
    )").arg(kButtonRadius).arg(kInputHeight));
    
    setFixedSize(kInputHeight, kInputHeight);
    
    auto* glow = new QGraphicsDropShadowEffect(this);
    glow->setBlurRadius(0);
    glow->setColor(Qt::transparent);
    glow->setOffset(0, 0);
    setGraphicsEffect(glow);
}

void SettingsIconButton::enterEvent(QEnterEvent* event) {
    QToolButton::enterEvent(event);
    startHoverTimer();
}

void SettingsIconButton::leaveEvent(QEvent* event) {
    QToolButton::leaveEvent(event);
    cancelHover();
    performHoverAnimation(false);
}

void SettingsIconButton::startHoverTimer() {
    if (hoverTimer_) {
        hoverTimer_->stop();
        delete hoverTimer_;
    }
    
    hoverTimer_ = new QTimer(this);
    hoverTimer_->setSingleShot(true);
    connect(hoverTimer_, &QTimer::timeout, [this]() {
        if (underMouse()) {
            performHoverAnimation(true);
        }
    });
    hoverTimer_->start(kHoverDelay);
}

void SettingsIconButton::cancelHover() {
    if (hoverTimer_) {
        hoverTimer_->stop();
        delete hoverTimer_;
        hoverTimer_ = nullptr;
    }
}

void SettingsIconButton::animateHover(bool entering) {
    if (hoverTimer_) {
        hoverTimer_->stop();
    }
    performHoverAnimation(entering);
}

void SettingsIconButton::performHoverAnimation(bool entering) {
    // Очистка предыдущих анимаций
    if (animationState_.glowAnimation) {
        animationState_.glowAnimation->stop();
        delete animationState_.glowAnimation;
    }
    if (animationState_.colorAnimation) {
        animationState_.colorAnimation->stop();
        delete animationState_.colorAnimation;
    }

    auto* glow = qobject_cast<QGraphicsDropShadowEffect*>(graphicsEffect());
    if (!glow) return;

    animationState_.glowAnimation = new QPropertyAnimation(glow, "blurRadius", this);
    animationState_.glowAnimation->setDuration(kButtonHoverAnimationDuration);
    animationState_.glowAnimation->setEasingCurve(QEasingCurve::OutQuad);
    animationState_.glowAnimation->setStartValue(glow->blurRadius());
    animationState_.glowAnimation->setEndValue(entering ? 15 : 0);

    animationState_.colorAnimation = new QPropertyAnimation(glow, "color", this);
    animationState_.colorAnimation->setDuration(kButtonHoverAnimationDuration);
    animationState_.colorAnimation->setEasingCurve(QEasingCurve::OutQuad);
    animationState_.colorAnimation->setStartValue(glow->color());
    animationState_.colorAnimation->setEndValue(entering ? QColor(255, 255, 255, 180) : QColor(0, 0, 0, 0));

    animationState_.glowAnimation->start();
    animationState_.colorAnimation->start();
}