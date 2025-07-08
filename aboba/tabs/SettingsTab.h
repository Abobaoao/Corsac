#pragma once

#include "BaseTab.h"
#include "SettingsHelper/SettingsSearchPanel.h"
#include "SettingsHelper/SettingsSectionWidget.h"
#include <QScrollArea>  
#include <QScroller>
#include <QPropertyAnimation>
#include <QWheelEvent>

class SettingsTab : public BaseTab {
    Q_OBJECT
public:
    SettingsTab(QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent* ev) override;

private:
    void setupMainLayout();
    void setupScroller();
    void populateSettings();
    void handleWheelEvent(QWheelEvent* event);
    QFont loadCustomFont();
    
    QScrollArea* scrollArea_ = nullptr;
    SettingsSearchPanel* searchPanel_ = nullptr;
    
    static const QString kScrollAreaStyle;
};