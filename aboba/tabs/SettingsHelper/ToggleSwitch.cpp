#include "ToggleSwitch.h"
#include <QPainter>
#include <QMouseEvent>

ToggleSwitch::ToggleSwitch(QWidget* parent) : QWidget(parent), m_anim(new QPropertyAnimation(this, "offset", this)) {
    setCursor(Qt::PointingHandCursor);
    m_anim->setDuration(150);
    m_anim->setEasingCurve(QEasingCurve::InOutQuad);
}

void ToggleSwitch::mousePressEvent(QMouseEvent*) {
    m_checked = !m_checked;
    emit toggled(m_checked);

    m_anim->stop();
    m_anim->setStartValue(m_offset);
    m_anim->setEndValue(m_checked ? 1.0 : 0.0);
    m_anim->start();
}

void ToggleSwitch::setOffset(qreal value) {
    m_offset = value;
    update();
}

void ToggleSwitch::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // фон
    QColor bg = m_checked ? QColor("#ffffff") : QColor("#000000");
    QColor knob = m_checked ? QColor("#000000") : QColor("#ffffff");

    p.setBrush(bg);
    p.setPen(Qt::NoPen);
    QRectF r(0, 0, width(), height());
    p.drawRoundedRect(r, height() / 2, height() / 2);

    // кнопка
    qreal diameter = height() - 4;
    qreal x = 2 + m_offset * (width() - diameter - 4);
    p.setBrush(knob);
    p.drawEllipse(QRectF(x, 2, diameter, diameter));
}
void ToggleSwitch::setChecked(bool checked) {
    if (m_checked == checked)
        return;

    m_checked = checked;
    emit toggled(m_checked);

    m_anim->stop();
    m_anim->setStartValue(m_offset);
    m_anim->setEndValue(m_checked ? 1.0 : 0.0);
    m_anim->start();
}
