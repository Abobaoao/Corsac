#include "SettingsSectionWidget.h"
#include "ToggleSwitch.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

SettingsSectionWidget::SettingsSectionWidget(const QString& title, 
                                             const QFont& headerFont, 
                                             const QList<SettingItem>& items,
                                             QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);
    
    auto* header = new QLabel(title);
    header->setFont(headerFont);
    header->setStyleSheet("color: #808080; padding-left: 25px; margin-bottom: 10px;");
    layout->addWidget(header);

    for (const auto& item : items) {
        layout->addLayout(createSettingItem(item));
        layout->addLayout(createDivider());
    }
}

QHBoxLayout* SettingsSectionWidget::createSettingItem(const SettingItem& setting) {
    auto* layout = new QHBoxLayout();
    
    auto* textLayout = new QVBoxLayout();
    auto* titleLabel = new QLabel(setting.title);
    auto* descLabel = new QLabel(setting.description);
    
    titleLabel->setStyleSheet("font-size:16px; color:white;");
    descLabel->setStyleSheet("font-size:12px; color:#999;");
    
    textLayout->addWidget(titleLabel);
    textLayout->addWidget(descLabel);
    
    auto* toggle = new ToggleSwitch(); 
    if (setting.enabled)
        toggle->setChecked(setting.enabled);

    layout->addLayout(textLayout);
    layout->addWidget(toggle);
    layout->setAlignment(toggle, Qt::AlignRight | Qt::AlignVCenter);
    return layout;
}

QHBoxLayout* SettingsSectionWidget::createDivider() {
    auto* lineLayout = new QHBoxLayout();
    lineLayout->setContentsMargins(15, 8, 25, 0);
    
    auto* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: black; max-height: 1px;");
    
    lineLayout->addWidget(line);
    return lineLayout;
}
