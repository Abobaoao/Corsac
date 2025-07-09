
#include "BaseTab.h"

BaseTab::BaseTab(QWidget *parent) : QWidget(parent) {
    
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);
    setStyleSheet("border: none;");
}
