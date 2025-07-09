#pragma once
#include <QWidget>
#include <QPropertyAnimation>

class ToggleSwitch : public QWidget {
    Q_OBJECT  

    Q_PROPERTY(qreal offset READ offset WRITE setOffset)

public:
    explicit ToggleSwitch(QWidget* parent = nullptr);
    QSize sizeHint() const override { return {50, 25}; }

    void setChecked(bool checked);
    bool isChecked() const { return m_checked; }

signals:
    void toggled(bool checked);
    
protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;

private:
    bool m_checked = false;
    qreal m_offset = 0;
    QPropertyAnimation* m_anim;

    qreal offset() const { return m_offset; }
    void setOffset(qreal value);
};
