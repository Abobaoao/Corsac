#include "SidebarButtons.h"
#include <QIcon>
#include <QLabel>

SidebarButtons::SidebarButtons(QWidget *parent) : QWidget(parent) {
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 10);
    layout->setSpacing(0);
    setAttribute(Qt::WA_StyledBackground, true);
    initializeStyle();

    QLabel *footerLabel = new QLabel("Corsac 0.1", this);
    footerLabel->setAlignment(Qt::AlignCenter);
    footerLabel->setStyleSheet("color: #1f1f1f; font-size: 12px;");
    layout->addStretch();
    QFont font = footerLabel->font();
    font.setStretch(120); // 100 — норм, 110 — растянут
    footerLabel->setFont(font);
    layout->addWidget(footerLabel);
}

void SidebarButtons::initializeStyle() {
    int fontId = QFontDatabase::addApplicationFont("C:/Inside/Aboba/ProjectsCode/VSC/Flip/fonts/LuxoraGrotesk-Regular.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    customFont = QFont(fontFamily);
    customFont.setPointSize(13);
    
}

QPushButton* SidebarButtons::addButton(const QString &text, int yPos, const QString &iconPath) {
    QPushButton *button = new QPushButton(this);
    button->setFixedSize(180, 40);
    button->move((width() - 180) / 2, yPos);

    // Создаем контейнер для содержимого кнопки
    QWidget *contentWidget = new QWidget(button);
    contentWidget->setStyleSheet("background: transparent;");
    QHBoxLayout *contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(3, 0, 0, 0); // Левый отступ 10px
    contentLayout->setSpacing(10); // Расстояние между иконкой и текстом 5px

    // Добавляем иконку
    if (!iconPath.isEmpty()) {
        QLabel *iconLabel = new QLabel(contentWidget);
        QPixmap pixmap(iconPath);
        iconLabel->setPixmap(pixmap.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        contentLayout->addWidget(iconLabel);
    }

    // Добавляем текст
    QLabel *textLabel = new QLabel(text, contentWidget);
    textLabel->setFont(customFont);
    textLabel->setStyleSheet("color: white;");
    contentLayout->addWidget(textLabel);
    contentLayout->addStretch();

    // Центрируем контейнер в кнопке
    QHBoxLayout *btnLayout = new QHBoxLayout(button);
    btnLayout->addWidget(contentWidget);
    btnLayout->setAlignment(Qt::AlignLeft);

    // Стилизация кнопки
    button->setStyleSheet(R"(
        
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.2); /* при наведении чуть ярче */
            border-radius: 17px;
        }
        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 0.15); /* при нажатии */
        }
        QPushButton:focus {
            outline: none;
        }    
    )");

    connect(button, &QPushButton::clicked, this, [=]() {
        emit buttonClicked(text);
    });

    buttons.append(button);
    return button;
}
