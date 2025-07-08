// AboutTab.cpp
#include "AboutTab.h"
#include <QLabel>
#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QPixmap>

QPixmap blurPixmap(const QPixmap& src, qreal radius) {
    if (src.isNull()) return QPixmap();

    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(src);
    QGraphicsBlurEffect blur;
    blur.setBlurRadius(radius);
    item.setGraphicsEffect(&blur);
    scene.addItem(&item);
    scene.setSceneRect(src.rect());

    QImage result(src.size(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    scene.render(&painter);
    return QPixmap::fromImage(result);
}

QPixmap adjustBrightness(const QPixmap& src, int delta) {
    if (src.isNull()) return QPixmap();

    QImage image = src.toImage().convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < image.height(); ++y) {
        QRgb* scanLine = reinterpret_cast<QRgb*>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x) {
            QRgb pixel = scanLine[x];
            int r = qMin(255, qMax(0, qRed(pixel) + delta));
            int g = qMin(255, qMax(0, qGreen(pixel) + delta));
            int b = qMin(255, qMax(0, qBlue(pixel) + delta));
            scanLine[x] = qRgba(r, g, b, qAlpha(pixel));
        }
    }

    return QPixmap::fromImage(image);
}

AboutTab::AboutTab(QWidget *parent) : BaseTab(parent) {
    QWidget *panel = new QWidget(this);
    panel->setGeometry(50, 70, 600, 400);
    panel->setStyleSheet("background-color: black; border-radius: 30px;");

    QLabel *corsacLabel = new QLabel("Corsac", panel);
    corsacLabel->move(50, 20);
    int corsacfont = QFontDatabase::addApplicationFont("C:/Inside/Aboba/ProjectsCode/VSC/Flip/fonts/LuxoraGrotesk-Regular.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(corsacfont).at(0);
    QFont corsacustom(fontFamily);
    corsacustom.setPointSize(24);
    corsacLabel->setFont(corsacustom);
    corsacLabel->setStyleSheet("color: white; background-color: transparent;");

    QGraphicsDropShadowEffect *textShadow = new QGraphicsDropShadowEffect();
    textShadow->setBlurRadius(25);
    textShadow->setColor(QColor(255, 255, 255, 255));
    textShadow->setOffset(0, 0);
    corsacLabel->setGraphicsEffect(textShadow);

    QLabel *imagelabel = new QLabel(panel);
    QPixmap original("C:/Inside/Aboba/ProjectsCode/VSC/Flip/resources/corsaclogowhite300x300.png");
    QPixmap blurred = blurPixmap(original.scaled(330, 330, Qt::KeepAspectRatio), 15);
    QPixmap darkened = adjustBrightness(blurred, -153);
    imagelabel->setPixmap(darkened); 
    imagelabel->setFixedSize(blurred.size());
    imagelabel->setStyleSheet("background-color: transparent;");
    int xCenter = (panel->width() - blurred.width()) / 2;
    int yCenter = (panel->height() - blurred.height()) / 2;
    imagelabel->move(xCenter, yCenter);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setBlurRadius(20);
    shadowEffect->setColor(QColor(255, 255, 255, 200));
    shadowEffect->setOffset(0, 0);
    imagelabel->setGraphicsEffect(shadowEffect);

    QLabel *versionLabel = new QLabel(panel);
    versionLabel->setText("Версия: 0.1");
    versionLabel->setGeometry(40, 70, 200, 20);
    versionLabel->setStyleSheet("color: white; font-size: 14px; background-color: transparent;");

    QLabel *authorLabel = new QLabel(panel);
    authorLabel->setText("Автор: Corsac");
    authorLabel->setGeometry(40, 90, 200, 20);
    authorLabel->setStyleSheet("color: white; font-size: 14px; background-color: transparent;");

    QLabel *aboutLabel = new QLabel(panel);
    aboutLabel->setText("[ О программе ]\nCorsac — это модульная программа, предназначенная для использования пользовательских плагинов.\nОсновной принцип работы — обеспечение быстрого доступа к функционалу через консоль и упрощение выполнения типовых задач на компьютере.");
    aboutLabel->setGeometry(40, 130, 520, 100);
    aboutLabel->setWordWrap(true);
    aboutLabel->setStyleSheet("color: white; font-size: 14px; background-color: transparent;");

    QLabel *contactsLabel = new QLabel(panel);
    contactsLabel->setText(
        "<span style='color: white;'>[ Контакты ]<br>Telegram: </span>"
        "<a href='https://t.me/corrssac' style='color: white;'>@corrssac</a>"
    );
    contactsLabel->setGeometry(40, 240, 400, 50);
    contactsLabel->setOpenExternalLinks(true);
    contactsLabel->setTextFormat(Qt::RichText);
    contactsLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    contactsLabel->setStyleSheet("font-size: 14px; background-color: transparent;");

    QLabel *additionalInfoLabel = new QLabel(panel);
    additionalInfoLabel->setText("[ Дополнительно ]\nПри обнаружении ошибок или сбое в работе программы рекомендуется сообщить об этом автору.");
    additionalInfoLabel->setGeometry(40, 280, 440, 100);
    additionalInfoLabel->setWordWrap(true);
    additionalInfoLabel->setStyleSheet("color: white; font-size: 14px; background-color: transparent;");
}
