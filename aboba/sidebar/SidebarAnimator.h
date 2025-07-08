#ifndef SIDEBARANIMATOR_H
#define SIDEBARANIMATOR_H

#include <QObject>
#include <QWidget>
#include <QPropertyAnimation>
#include <QGraphicsEffect>
#include <QParallelAnimationGroup>

class SidebarAnimator : public QObject {
    Q_OBJECT
public:
    SidebarAnimator(QWidget *sidebar, int targetWidth = 150, QObject *parent = nullptr);
    
    void showSidebar();
    void hideSidebar();
    bool isVisible() const;

private:
    QWidget *m_sidebar;
    int m_targetWidth;
    bool m_visible;
    
    // Анимационные элементы
    QGraphicsOpacityEffect *m_opacityEffect;
    QPropertyAnimation *m_geometryAnimation;
    QPropertyAnimation *m_opacityAnimation;
    QParallelAnimationGroup *m_animationGroup;
};

#endif // SIDEBARANIMATOR_H