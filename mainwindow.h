#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <vector>
#include <QRandomGenerator> 
#include <QtMath> 
#include <QStackedWidget>
#include "aboba/console/Console.h"
#include "aboba/sidebar/SidebarAnimator.h"
#include "aboba/sidebar/SidebarButtons.h" 
#include "aboba/tabs/AboutTab.h"
#include "aboba/tabs/SettingsTab.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT 

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override; 
    void keyPressEvent(QKeyEvent *event) override;
    void setupSidebar(); 
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QWidget *panel;
    bool dragging;   
    QPoint offset; 
    Console *console;
    AboutTab *aboutTab;
    SettingsTab *settingsTab;
    SidebarAnimator *sidebarAnim;
    SidebarButtons *sidebar;
    QStackedWidget *stackedWidget;
    QWidget *consoleContainer;
    QWidget* sidebarBackground;
    QLabel *titleLabel;
};

#endif 
