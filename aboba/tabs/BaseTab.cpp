// BaseTab.cpp
#include "BaseTab.h"

BaseTab::BaseTab(QWidget *parent) : QWidget(parent) {
    // Можно оставить полностью прозрачным
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);
    setStyleSheet("border: none;");
}
