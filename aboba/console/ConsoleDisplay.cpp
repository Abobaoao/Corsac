
#include "ConsoleDisplay.h"
#include <QFontDatabase>
#include <QTextCursor>

ConsoleDisplay::ConsoleDisplay(QWidget *parent) : QTextEdit(parent) {
    initialize();
}

void ConsoleDisplay::initialize() {
    
    int fontId = QFontDatabase::addApplicationFont("C:/Inside/Aboba/ProjectsCode/VSC/Flip/fonts/FiraCode-Regular.ttf");
    QFont firaFont("Fira Code");
    firaFont.setPointSize(12);
    setFont(firaFont);

    
    setStyleSheet(R"(
        QTextEdit {
            background-color: #0d0d0d;
            color: #e0e0e0;
            font-family: 'Fira Code';
            font-size: 12px;
            border: none;
            border-radius: 15px;
            padding: 15px;
            font-feature-settings: "liga" on, "calt" on;
            selection-background-color: rgb(169, 169, 169);
            selection-color: black;
        }
    )");
    setReadOnly(false);
    setTextInteractionFlags(Qt::TextEditorInteraction);
}

void ConsoleDisplay::printMessage(const QString &message) {
    static int lineNumber = 1;
    QString lineNumberText = QString("<span style='color:#4d4d4d'>%1</span>")
                                .arg(lineNumber++, 3, 10, QLatin1Char(' '));
    append(lineNumberText + "  " + message);
    moveCursor(QTextCursor::End);
}