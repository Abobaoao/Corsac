// AboutTab.h
#pragma once
#include "BaseTab.h"
#include <QFont>
#include <QFontDatabase>

class AboutTab : public BaseTab {
    Q_OBJECT
public:
    explicit AboutTab(QWidget *parent = nullptr);
private:
    QFont corsacustom;
};
