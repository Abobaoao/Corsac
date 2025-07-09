
#ifndef CONSOLE_H
#define CONSOLE_H

#include "ConsoleDisplay.h"
#include "ConsoleLog.h"
#include "CommandInput.h"
#include "CommandSuggestion.h"
#include <QTextEdit>
#include <QEvent>

class Console : public ConsoleDisplay {
    Q_OBJECT
public:
    enum MessageType {
        Error,
        Correct,
        ConsoleText,
        UserInput
    };

    explicit Console(QWidget *parent = nullptr);
    void printMessage(const QString &message, MessageType type);
    void prompt();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void moveEvent(QMoveEvent *event) override {
        m_suggestions->hideSuggestions();
        QTextEdit::moveEvent(event);
    }
    
    void hideEvent(QHideEvent *event) override {
        m_suggestions->hideSuggestions();
        QTextEdit::hideEvent(event);
    }
    
    void changeEvent(QEvent *event) override {
        if (event->type() == QEvent::WindowStateChange) {
            if (isMinimized())
                m_suggestions->hideSuggestions();
        }
        QTextEdit::changeEvent(event);
    }

private:
    QString inputBuffer;
    int promptPosition;
    ConsoleLog* logManager;
    CommandInput* inputHandler;
    private:
    CommandSuggestion* m_suggestions;
    QString m_ghostText;
    QListWidget* m_suggestionWidget;
    
    void updateSuggestions();
    void acceptSuggestion();
    void updateGhostText();
};

#endif 