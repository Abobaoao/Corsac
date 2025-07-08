#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QMovie>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QPropertyAnimation>
#include <QRandomGenerator>

// Создаем подкласс QSplashScreen, игнорирующий события мыши
class CustomSplashScreen : public QSplashScreen {
public:
    using QSplashScreen::QSplashScreen;
protected:
    void mousePressEvent(QMouseEvent*) override {} // Игнорируем клики
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.setWindowOpacity(0.0);
    mainWindow.show();

    // Используем кастомный сплэш-скрин
    CustomSplashScreen *loadingWindow = new CustomSplashScreen();
    loadingWindow->setPixmap(QPixmap(900, 600));
    loadingWindow->setStyleSheet("background-color: black;");
    loadingWindow->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    QLabel *corsacmainLabel = new QLabel(loadingWindow);
    QPixmap corsacPixmap("C:/Inside/Aboba/ProjectsCode/VSC/Flip/resources/corsaclogowhite200x200.png");
    corsacmainLabel->setPixmap(corsacPixmap.scaled(200, 200, Qt::KeepAspectRatio));
    corsacmainLabel->setStyleSheet("background-color: transparent;"); 
    corsacmainLabel->setGeometry((900 - 200) / 2, (600 - 200) / 2, 200, 200);

    QGraphicsDropShadowEffect *logomainEffect = new QGraphicsDropShadowEffect();
    logomainEffect->setBlurRadius(30);
    logomainEffect->setOffset(0, 0);
    corsacmainLabel->setGraphicsEffect(logomainEffect);

    QPropertyAnimation *shadowAnim = new QPropertyAnimation(logomainEffect, "color");
    shadowAnim->setDuration(500);  // Время анимации (1 секунда)

    // Таймер для цикличности анимации
    QTimer *colorChangeTimer = new QTimer();
    colorChangeTimer->setInterval(1000);  // Интервал в 2 секунды (цвет меняется каждые 2 секунды)

    QObject::connect(colorChangeTimer, &QTimer::timeout, [&]() {
        if (logomainEffect->color() == QColor(255, 255, 255, 120)) {
            // Анимируем изменение цвета на непрозрачный
            shadowAnim->setStartValue(QColor(255, 255, 255, 120));
            shadowAnim->setEndValue(QColor(255, 255, 255, 255));
        } else {
            // Анимируем изменение цвета на прозрачный
            shadowAnim->setStartValue(QColor(255, 255, 255, 255));
            shadowAnim->setEndValue(QColor(255, 255, 255, 120));
        }
        shadowAnim->start();  // Запуск анимации
    });

    // Запуск таймера
    colorChangeTimer->start();

    QLabel *consoleLabel = new QLabel(loadingWindow);
    consoleLabel->setStyleSheet("color: White ; font: 10pt Courier;");
    consoleLabel->setGeometry(20, 50, 300, 300);
    consoleLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QStringList textLines = {
        "> Booting system...",
        "> Checking hardware...",
        "> Verifying memory...",
        "> Scanning storage devices...",
        "> Establishing network connections...",
        "> Loading core modules...",
        "> Initializing security protocols...",
        "> Mounting file system...",
        "> Loading user preferences...",
        "> Checking dependencies...",
        "> Optimizing performance...",
        "> Finalizing startup...",
        "> System ready!"
    };
    QString currentText;
    int index = 0;

    QTimer *typingTimer = new QTimer();
    QObject::connect(typingTimer, &QTimer::timeout, [&]() {
        if (index < textLines.size()) {
            currentText += textLines[index] + "\n";
            consoleLabel->setText(currentText);
            index++;
            typingTimer->start(QRandomGenerator::global()->bounded(100, 400));
        } else {
            typingTimer->stop();
        }
    });
    typingTimer->start(QRandomGenerator::global()->bounded(100, 400));

    loadingWindow->move(loadingWindow->x(), loadingWindow->y() + 20);
    loadingWindow->show();

    QTimer::singleShot(1000, [&]() {
        mainWindow.setWindowOpacity(1.0);
    });

    QTimer::singleShot(2000, [=, &mainWindow]() {
        QPropertyAnimation *anim = new QPropertyAnimation(loadingWindow, "windowOpacity");
        colorChangeTimer->stop();
        colorChangeTimer->deleteLater();   
        shadowAnim->stop();
        shadowAnim->deleteLater();
        anim->setDuration(1000);
        anim->setStartValue(1.0);
        anim->setEndValue(0.0);
        anim->start(QAbstractAnimation::DeleteWhenStopped);

        QObject::connect(anim, &QPropertyAnimation::finished, [=, &mainWindow]() {
            loadingWindow->close();
            loadingWindow->deleteLater();
            typingTimer->deleteLater();
            mainWindow.activateWindow();
        });
    });

    return app.exec();
}
