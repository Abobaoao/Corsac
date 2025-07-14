#include "ToggleSwitch.h"
#include <QPainter>
#include <QMouseEvent>

ToggleSwitch::ToggleSwitch(QWidget* parent)
    : QWidget(parent),
      m_anim(new QPropertyAnimation(this, "offset", this)),
      m_glowAnim(new QPropertyAnimation(this, "glowOpacity", this)),
      m_glowEffect(new QGraphicsDropShadowEffect(this))
{
    setCursor(Qt::PointingHandCursor);
    setMinimumSize(50, 25);

    m_anim->setDuration(150);
    m_anim->setEasingCurve(QEasingCurve::InOutQuad);

    m_glowEffect->setBlurRadius(20);
    m_glowEffect->setOffset(0, 0);
    m_glowEffect->setColor(QColor(255, 255, 255, 0));  
    setGraphicsEffect(m_glowEffect);

    m_glowAnim->setDuration(600);
    m_glowAnim->setEasingCurve(QEasingCurve::InOutQuad);
    m_glowAnim->setLoopCount(1);

    setChecked(m_checked);
}

void ToggleSwitch::mousePressEvent(QMouseEvent*) {
    setChecked(!m_checked);
}

void ToggleSwitch::setOffset(qreal value) {
    m_offset = value;
    update();
}

void ToggleSwitch::setGlowOpacity(qreal value) {
    m_glowOpacity = value;
    QColor col = m_glowEffect->color();
    col.setAlphaF(m_glowOpacity);
    m_glowEffect->setColor(col);
    update();
}

void ToggleSwitch::setChecked(bool checked) {
    if (m_checked == checked)
        return;

    m_checked = checked;
    emit toggled(m_checked);

    qreal startOffset = (m_anim->state() == QAbstractAnimation::Running)
        ? m_anim->currentValue().toReal()
        : m_offset;

    m_anim->stop();
    m_anim->setStartValue(startOffset);
    m_anim->setEndValue(m_checked ? 1.0 : 0.0);
    m_anim->setDuration(400);
    m_anim->start();

    qreal startGlow = (m_glowAnim->state() == QAbstractAnimation::Running)
        ? m_glowAnim->currentValue().toReal()
        : m_glowOpacity;

    m_glowAnim->stop();
    m_glowAnim->setStartValue(startGlow);
    m_glowAnim->setEndValue(m_checked ? 1.0 : 0.0);
    m_glowAnim->start();
}

void ToggleSwitch::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const QColor bg   = m_checked ? QColor("#c7c7c7") : QColor("#121212");
    const QColor knob = m_checked ? QColor("#000000") : QColor("#ffffff");

    p.setBrush(bg);
    p.setPen(Qt::NoPen);
    QRectF r(0, 0, width(), height());
    p.drawRoundedRect(r, height() / 2.0, height() / 2.0);

    qreal dia = height() - 4;
    qreal x = 2 + m_offset * (width() - dia - 4);
    p.setBrush(knob);
    p.drawEllipse(QRectF(x, 2, dia, dia));
}