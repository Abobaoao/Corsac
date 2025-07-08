#include "mainwindow.h"
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QFontDatabase>
#include <QLabel>
#include <QPixmap>
#include <QIcon>
#include <QPushButton>
#include <QTextEdit>
#include <QGraphicsEllipseItem>
#include <QRandomGenerator>
#include <QtMath>
#include "aboba/console/Console.h"
#include "aboba/sidebar/SidebarButtons.h"
#include "aboba/tabs/AboutTab.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      dragging(false)
{
    setWindowFlag(Qt::FramelessWindowHint);
    setFixedSize(900, 600);
    setStyleSheet("background-color: #1f1f1f; border: none; ");

    panel = new QWidget(this);
    panel->setStyleSheet("background-color: black");
    panel->setFixedHeight(40);
    panel->setGeometry(0, 0, width(), 40);

    QLabel *logoLabel = new QLabel(panel);
    QPixmap logoPixmap("C:/Inside/Aboba/ProjectsCode/VSC/Flip/resources/corsaclogowhite40x40.png");
    logoLabel->setPixmap(logoPixmap.scaled(40, 40, Qt::KeepAspectRatio));
    logoLabel->setStyleSheet("background-color: transparent;"); 
    logoLabel->setGeometry(0, 0, 40, 40);

    QGraphicsDropShadowEffect *logoEffect = new QGraphicsDropShadowEffect();
    logoEffect->setBlurRadius(20);
    logoEffect->setColor(QColor(255, 255, 255, 255));
    logoEffect->setOffset(0, 0);
    logoLabel->setGraphicsEffect(logoEffect);

    QPushButton *menuButton = new QPushButton(panel);
    menuButton->setFixedSize(35, 30);
    QPixmap menuPixmap("C:/Inside/Aboba/ProjectsCode/VSC/Flip/resources/linemenu.png");
    menuButton->setIcon(QIcon(menuPixmap));
    menuButton->setIconSize(QSize(40, 25)); //размер кнопки
    menuButton->setStyleSheet(
        "QPushButton { background-color: black; border: none; outline: none; }"
        "QPushButton:focus { outline: none; }"
        "QPushButton:hover { background-color: rgba(56, 56, 56, 1); border-radius: 5px; }" // Цветrgb(56, 56, 56) с 50% прозрачностью и округление углов
    );
    int menuButtonY = (panel->height() - menuButton->height()) / 2;
    menuButton->move(logoLabel->geometry().right() + 3, menuButtonY);

    titleLabel = new QLabel("Corsac | console", panel);
    int fontId = QFontDatabase::addApplicationFont("C:/Inside/Aboba/ProjectsCode/VSC/Flip/fonts/minecraft.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont titleFont(fontFamily);
    titleFont.setPointSize(16);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: white;");
    titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter); 
    titleLabel->setFixedSize(280, 30); 
    titleLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    int centerX = (panel->width() - titleLabel->width()) / 2;
    int centerY = (panel->height() - titleLabel->height()) / 2;
    titleLabel->move(centerX + 38, centerY);

    QPushButton *closeButton = new QPushButton(panel);
    closeButton->setFixedSize(40, 40);

    QPixmap closePixmap("C:/Inside/Aboba/ProjectsCode/VSC/Flip/resources/cross.png");
    closeButton->setIcon(QIcon(closePixmap));
    closeButton->setIconSize(QSize(30, 30)); // Размер иконки

    closeButton->setFocusPolicy(Qt::NoFocus);
    closeButton->setStyleSheet(
    "QPushButton { background-color: black; border: none; margin: 0px; padding: 0px; }"
    "QPushButton:hover { background-color: rgba(56, 56, 56, 1); }"
);
    closeButton->move(panel->width() - closeButton->width(), 0);
    connect(closeButton, &QPushButton::clicked, this, &MainWindow::close);
    setupSidebar();

    connect(menuButton, &QPushButton::clicked, [=]() {
        if (sidebarAnim->isVisible()) {
            stackedWidget->setCurrentWidget(consoleContainer);
            stackedWidget->setGeometry(0, 40, width(), height() - 40);
            sidebarAnim->hideSidebar();          
        } else {
            sidebarAnim->showSidebar();
            sidebar->raise();
        }      
    });
      
    console->viewport()->installEventFilter(this);
    this->installEventFilter(this);;
    
    panel->installEventFilter(this);
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{  
    if (sidebarAnim && sidebarAnim->isVisible())
    {
        if (event->key() == Qt::Key_Escape)
        {
            stackedWidget->setCurrentWidget(consoleContainer);
            stackedWidget->setGeometry(0, 40, width(), height() - 40);
            sidebarAnim->hideSidebar();
            return;
        }
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::setupSidebar() {
    int sidebarWidth = 200;

    sidebar = new SidebarButtons(this);
    sidebar->setStyleSheet("background-color: black; border: none;");
    sidebar->setGeometry(-sidebarWidth, 40, sidebarWidth, height() - 40);

    sidebarAnim = new SidebarAnimator(sidebar, sidebarWidth);
    sidebarAnim->hideSidebar();

    aboutTab = new AboutTab(this);
    console = new Console(this);
    settingsTab = new SettingsTab(this);

    consoleContainer = new QWidget(); 
    QVBoxLayout *consoleLayout = new QVBoxLayout(consoleContainer);
    consoleLayout->setContentsMargins(20, 30, 20, 20);
    consoleLayout->addWidget(console);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->setGeometry(0, 40, width(), height() - 40);
    stackedWidget->addWidget(consoleContainer);
    stackedWidget->addWidget(aboutTab);
    stackedWidget->addWidget(settingsTab);

    sidebar->addButton("about", 475, "C:/Inside/Aboba/ProjectsCode/VSC/Flip/resources/info.png");
    sidebar->addButton("settings", 430, "C:/Inside/Aboba/ProjectsCode/VSC/Flip/resources/settings.png");
    sidebar->addButton("</> console", 20, "C:/Inside/Aboba/ProjectsCode/VSC/Flip/resources/console.png");

    connect(sidebar, &SidebarButtons::buttonClicked, this, [=](const QString &id){
        if (id == "</> console") {
            stackedWidget->setCurrentWidget(consoleContainer);
            stackedWidget->setGeometry(0, 40, width(), height() - 40);
            sidebarAnim->hideSidebar();
            titleLabel->setText("Corsac | console");
        } else if (id == "about") {
            stackedWidget->setCurrentWidget(aboutTab);
            stackedWidget->setGeometry(200, 40, width() - 200, height() - 40);
            sidebarAnim->showSidebar();
            titleLabel->setText("Corsac | about");
        } else if (id == "settings") {
            stackedWidget->setCurrentWidget(settingsTab);
            stackedWidget->setGeometry(200, 40, width() - 200, height() - 40);
            sidebarAnim->showSidebar();
            titleLabel->setText("Corsac | settings");
        }
    });
    
    
}
MainWindow::~MainWindow()
{
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress && sidebarAnim->isVisible())
    {
        if (stackedWidget->currentWidget() == consoleContainer)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint globalPos = mouseEvent->globalPosition().toPoint();

            QPoint sidebarPos = sidebar->mapFromGlobal(globalPos);
            QRect sidebarRect = sidebar->geometry();

            if (!sidebarRect.contains(sidebarPos)) {
                sidebarAnim->hideSidebar();
                stackedWidget->setCurrentWidget(consoleContainer);
                stackedWidget->setGeometry(0, 40, width(), height() - 40);
                return true; // Блокируем дальнейшую обработку
            }
        }
    }

    if (watched == panel) 
    {
        if (event->type() == QEvent::MouseButtonPress) 
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) 
            {
                dragging = true;
                offset = mouseEvent->globalPosition().toPoint() - frameGeometry().topLeft();
            }
        } 
        else if (event->type() == QEvent::MouseMove) 
        {
            if (dragging) 
            {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
                move(mouseEvent->globalPosition().toPoint() - offset);
            }
        } 
        else if (event->type() == QEvent::MouseButtonRelease) 
        {
            dragging = false;
        }
    }

    return QMainWindow::eventFilter(watched, event);
}