#include "SettingsSearchPanel.h"
#include <QHBoxLayout>
#include <QGraphicsBlurEffect>
#include <QPropertyAnimation>
#include "SettingsIconButton.h"

SettingsSearchPanel::SettingsSearchPanel(QWidget* parent) 
    : QWidget(parent) 
{
    setFixedHeight(kGlassHeight);
    setupGlassBackground();
    setupSearchControls();
}

void SettingsSearchPanel::setupGlassBackground() {
    auto* glassBg = new QWidget(this);
    glassBg->setStyleSheet("background-color: rgba(0,0,0,100); border-radius: 20px;");
    glassBg->setGeometry(0, 0, width() + 600, kGlassHeight - 10);
    glassBg->lower();
    
    auto* blur = new QGraphicsBlurEffect(glassBg);
    blur->setBlurRadius(18);
    glassBg->setGraphicsEffect(blur);
}

void SettingsSearchPanel::setupSearchControls() {
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(kMargin, kMargin - 10, kMargin, kMargin);
    layout->setSpacing(kIconSpacing);
    
    searchInput_ = new QLineEdit(this);
    searchInput_->setPlaceholderText("Поиск");
    searchInput_->setStyleSheet(kSearchInputStyle);
    searchInput_->setFixedHeight(kInputHeight);
    layout->addWidget(searchInput_);
    layout->addStretch();

    btnThemes_ = createIconButton("🎨", "Темы");
    btnPlugins_ = createIconButton("🧩", "Плагины");
    
    layout->addWidget(btnThemes_);
    layout->addWidget(btnPlugins_);
    btnThemes_->installEventFilter(this);
    btnPlugins_->installEventFilter(this);
}

QToolButton* SettingsSearchPanel::createIconButton(const QString& icon, const QString& tooltip) {
    auto* button = new SettingsIconButton(icon, tooltip, this);
    button->setText(icon);
    button->setToolTip(tooltip);
    button->setStyleSheet(QString(R"(
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
    
    button->setFixedSize(kInputHeight, kInputHeight);
    return button;
}

void SettingsSearchPanel::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    const int minInitialWidth = 560;
    int hintWidth = searchInput_->sizeHint().width();
    originalSearchWidth_ = std::max(minInitialWidth, hintWidth);
    searchInput_->setFixedWidth(originalSearchWidth_);
}

void SettingsSearchPanel::animateSearchWidth(int newWidth) {
    auto* anim = new QPropertyAnimation(searchInput_, "minimumWidth", this);
    anim->setDuration(150);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->setStartValue(searchInput_->minimumWidth());
    anim->setEndValue(newWidth);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

