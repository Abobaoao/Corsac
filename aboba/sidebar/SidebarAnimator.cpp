#include "SidebarAnimator.h"

SidebarAnimator::SidebarAnimator(QWidget *sidebar, int targetWidth, QObject *parent)
    : QObject(parent), m_sidebar(sidebar), m_targetWidth(targetWidth), m_visible(false)
{
    
    m_opacityEffect = new QGraphicsOpacityEffect(m_sidebar);
    m_sidebar->setGraphicsEffect(m_opacityEffect);
    
    
    m_geometryAnimation = new QPropertyAnimation(m_sidebar, "geometry", this);
    
    
    m_opacityAnimation = new QPropertyAnimation(m_opacityEffect, "opacity", this);
    
    
    m_animationGroup = new QParallelAnimationGroup(this);
    m_animationGroup->addAnimation(m_geometryAnimation);
    m_animationGroup->addAnimation(m_opacityAnimation);
    
    
    m_geometryAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    m_opacityAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    
    
    m_geometryAnimation->setDuration(350);
    m_opacityAnimation->setDuration(350);
    
    
    m_sidebar->setGeometry(-m_targetWidth, 40, m_targetWidth, sidebar->parentWidget()->height() - 40);
    m_opacityEffect->setOpacity(0.0);
}

void SidebarAnimator::showSidebar() {
    m_animationGroup->stop();
    
    m_geometryAnimation->setStartValue(m_sidebar->geometry());
    m_geometryAnimation->setEndValue(QRect(0, 40, m_targetWidth, m_sidebar->height()));
    
    m_opacityAnimation->setStartValue(m_opacityEffect->opacity());
    m_opacityAnimation->setEndValue(1); 
    
    m_animationGroup->start();
    m_visible = true;
}

void SidebarAnimator::hideSidebar() {
    m_animationGroup->stop();
    
    m_geometryAnimation->setStartValue(m_sidebar->geometry());
    m_geometryAnimation->setEndValue(QRect(-m_targetWidth, 40, m_targetWidth, m_sidebar->height()));
    
    m_opacityAnimation->setStartValue(m_opacityEffect->opacity());
    m_opacityAnimation->setEndValue(0.0);
    
    m_animationGroup->start();
    m_visible = false;
}

bool SidebarAnimator::isVisible() const {
    return m_visible;
}