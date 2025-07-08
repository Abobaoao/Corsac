#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFont>
#include <QFontDatabase>
#include <QList>

class SidebarButtons : public QWidget {
    Q_OBJECT

public:
    explicit SidebarButtons(QWidget *parent = nullptr);
    QPushButton* addButton(const QString &text, int yPos, const QString &iconPath = "");
    
signals:
    void buttonClicked(const QString &buttonName);

private:
    QVBoxLayout *layout;
    QFont customFont;
    QList<QPushButton*> buttons;

    void initializeStyle();
};
