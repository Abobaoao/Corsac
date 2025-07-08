#pragma once

#include <QWidget>
#include <QFont>
#include <QList>
#include <QHBoxLayout>

struct SettingItem {
    QString title;
    QString description;
    bool enabled;
};

class SettingsSectionWidget : public QWidget {
    Q_OBJECT
public:
    SettingsSectionWidget(const QString& title, 
                        const QFont& headerFont, 
                        const QList<SettingItem>& items,
                        QWidget* parent = nullptr);
    
private:
    QHBoxLayout* createSettingItem(const SettingItem& setting);
    QHBoxLayout* createDivider();
    
    // Константы
    static constexpr int kMargin = 20;
    static constexpr int kMainSpacing = 16;
    static constexpr int kSectionSpacing = 0;
};