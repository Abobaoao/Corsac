#ifndef TOGGLESWITCH_H
#define TOGGLESWITCH_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>

class ToggleSwitch : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal offset READ offset WRITE setOffset)
    Q_PROPERTY(qreal glowOpacity READ glowOpacity WRITE setGlowOpacity)

public:
    explicit ToggleSwitch(QWidget* parent = nullptr);

    qreal offset() const { return m_offset; }
    void setOffset(qreal value);

    qreal glowOpacity() const { return m_glowOpacity; }
    void setGlowOpacity(qreal value);

    bool isChecked() const { return m_checked; }
    void setChecked(bool checked);

signals:
    void toggled(bool checked);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    bool m_checked = false;
    qreal m_offset = 0.0;
    qreal m_glowOpacity = 0.0;

    QPropertyAnimation* m_anim;
    QPropertyAnimation* m_glowAnim;
    QGraphicsDropShadowEffect* m_glowEffect;
};

#endif // TOGGLESWITCH_H
