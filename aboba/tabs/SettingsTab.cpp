#include "SettingsTab.h"
#include <QVBoxLayout>
#include <QScroller>
#include <QPropertyAnimation>
#include <QFontDatabase>
#include <QScrollBar>

const QString SettingsTab::kScrollAreaStyle = R"(
    QScrollArea { 
        background: transparent; 
        border: none; 
        padding-right: 20px; 
    }
    QScrollArea > QWidget { 
        background: transparent; 
    }
    QScrollBar:vertical {
        background: transparent; 
        width: 8px; 
        margin: 20px 0; 
        position: absolute; 
        right: -20px;
    }
    QScrollBar::handle:vertical { 
        background: #555; 
        min-height: 30px; 
        border-radius: 4px; 
    }
    QScrollBar::add-page, 
    QScrollBar::sub-page { 
        background: transparent; 
    }
    QScrollBar::add-line, 
    QScrollBar::sub-line { 
        height: 0; 
    }
)";

SettingsTab::SettingsTab(QWidget* parent)
    : BaseTab(parent)
{
    setupMainLayout();
    setupScroller();
    populateSettings();
}

void SettingsTab::setupMainLayout() {
    auto* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);
    
    searchPanel_ = new SettingsSearchPanel(this);
    rootLayout->addWidget(searchPanel_);
    
    scrollArea_ = new QScrollArea(this);
    scrollArea_->setWidgetResizable(true);
    scrollArea_->setFrameShape(QFrame::NoFrame);
    scrollArea_->setStyleSheet(kScrollAreaStyle);

    auto* contentWrapper = new QWidget();
    contentWrapper->setStyleSheet("background: transparent;");
    contentWrapper->setContentsMargins(0, 20, 0, 20);
    
    auto* mainLayout = new QVBoxLayout(contentWrapper);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);
    
    scrollArea_->setWidget(contentWrapper);
    rootLayout->addWidget(scrollArea_);
}

void SettingsTab::setupScroller() {
    auto* scroller = QScroller::scroller(scrollArea_->viewport());
    QScroller::grabGesture(scrollArea_->viewport(), QScroller::LeftMouseButtonGesture);
    
    auto props = scroller->scrollerProperties();
    props.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
    props.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
    props.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.5);
    props.setScrollMetric(QScrollerProperties::OvershootDragDistanceFactor, 0.5);
    props.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.5);
    props.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
    
    scroller->setScrollerProperties(props);
    scrollArea_->viewport()->installEventFilter(this);
}

void SettingsTab::populateSettings() {
    auto* contentWrapper = qobject_cast<QWidget*>(scrollArea_->widget());
    auto* mainLayout = qobject_cast<QVBoxLayout*>(contentWrapper->layout());
    
    auto headerFont = loadCustomFont();
    
    auto section1 = new SettingsSectionWidget("Система", headerFont, {
        {"Режим разработчика", "Включает возможности разработчика", false},
        {"Автозапуск при старте системы", "Запускает приложение при запуске ОС", false}
    }, contentWrapper);

    auto section2 = new SettingsSectionWidget("Интерфейс", headerFont, {
        {"Тёмная тема", "Активировать тёмный режим интерфейса", true},
        {"Упрощённый интерфейс", "Скрыть дополнительные элементы управления", false}
    }, contentWrapper);

    auto section3 = new SettingsSectionWidget("Безопасность", headerFont, {
        {"Запрашивать пароль при запуске", "Требует ввод пароля при старте приложения", false},
        {"Шифровать конфигурации", "Хранить настройки в зашифрованном виде", false},
        {"Отключить удалённый доступ", "Запрещает любые внешние подключения", true}
    }, contentWrapper);
    mainLayout->addWidget(section1);
    mainLayout->addWidget(section2);
    mainLayout->addWidget(section3);
    mainLayout->addStretch();
    setAttribute(Qt::WA_TranslucentBackground);
}

QFont SettingsTab::loadCustomFont() {
    int id = QFontDatabase::addApplicationFont(
        "C:/Inside/Aboba/ProjectsCode/VSC/Flip/fonts/LuxoraGrotesk-Regular.ttf");
    return QFont(QFontDatabase::applicationFontFamilies(id).value(0), 15);
}

bool SettingsTab::eventFilter(QObject* obj, QEvent* ev) {
    if (obj == scrollArea_->viewport() && ev->type() == QEvent::Wheel) {
        handleWheelEvent(static_cast<QWheelEvent*>(ev));
        return true;
    }
    
    if (obj == searchPanel_->themesButton() || obj == searchPanel_->pluginsButton()) {
        auto* button = qobject_cast<QToolButton*>(obj);
        if (ev->type() == QEvent::Enter) {
            searchPanel_->animateSearchWidth(searchPanel_->searchInput()->width() - 10);
            if (obj == searchPanel_->themesButton()) {
                auto* anim = new QPropertyAnimation(searchPanel_->pluginsButton(), "pos", this);
                anim->setDuration(200);
                anim->setStartValue(searchPanel_->pluginsButton()->pos());
                anim->setEndValue(searchPanel_->pluginsButton()->pos() + QPoint(10, 0));
                anim->start(QAbstractAnimation::DeleteWhenStopped);
            } else {
                auto* anim = new QPropertyAnimation(searchPanel_->themesButton(), "pos", this);
                anim->setDuration(200);
                anim->setStartValue(searchPanel_->themesButton()->pos());
                anim->setEndValue(searchPanel_->themesButton()->pos() - QPoint(10, 0));
                anim->start(QAbstractAnimation::DeleteWhenStopped);
            }
        }
        else if (ev->type() == QEvent::Leave) {
            searchPanel_->animateSearchWidth(searchPanel_->searchInput()->width() + 10);
            if (obj == searchPanel_->themesButton()) {
                auto* anim = new QPropertyAnimation(searchPanel_->pluginsButton(), "pos", this);
                anim->setDuration(200);
                anim->setStartValue(searchPanel_->pluginsButton()->pos());
                anim->setEndValue(searchPanel_->pluginsButton()->pos() - QPoint(10, 0));
                anim->start(QAbstractAnimation::DeleteWhenStopped);
            } else {
                auto* anim = new QPropertyAnimation(searchPanel_->themesButton(), "pos", this);
                anim->setDuration(200);
                anim->setStartValue(searchPanel_->themesButton()->pos());
                anim->setEndValue(searchPanel_->themesButton()->pos() + QPoint(10, 0));
                anim->start(QAbstractAnimation::DeleteWhenStopped);
            }
        }
    }
    
    return BaseTab::eventFilter(obj, ev);
}

void SettingsTab::handleWheelEvent(QWheelEvent* event) {
    auto* bar = scrollArea_->verticalScrollBar();
    auto* anim = new QPropertyAnimation(bar, "value", this);
    
    anim->setDuration(500);
    anim->setStartValue(bar->value());
    anim->setEndValue(bar->value() - event->angleDelta().y());
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}