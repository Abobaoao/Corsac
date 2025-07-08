#include "Console.h"
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QTextBlock>
#include <QPainter>
#include <QFontDatabase>

Console::Console(QWidget *parent) : ConsoleDisplay(parent) {
    // Инициализация компонентов
    logManager = new ConsoleLog("console_log.txt");
    inputHandler = new CommandInput();
    m_suggestionWidget = new QListWidget(this);
    m_suggestionWidget->setAttribute(Qt::WA_ShowWithoutActivating);
    m_suggestions = new CommandSuggestion(m_suggestionWidget);

    // Первоначальный вывод
    QString version = "0.1";
    printMessage(
        QString("<span style='color:#ffd633'>Author:</span> <span style='color:White'>Corsac</span> | ")
        + QString("<span style='color:#ffd633'>Version:</span> <span style='color:White'>%1</span>").arg(version),
        ConsoleText
    );
    prompt();
    m_suggestionWidget = new QListWidget(this);
    m_suggestionWidget->setWindowFlags(Qt::Widget); 
    m_suggestionWidget->setParent(this, Qt::Popup);
    m_suggestions = new CommandSuggestion(m_suggestionWidget);
    
}

void Console::printMessage(const QString &message, MessageType type) {
    QString styledMessage;
    QString corsacPrefix = "<span style='color:#ffd633'>[</span><span style='color:White'>Corsac</span><span style='color:#ffd633'>]</span> ";
    
    switch (type) {
        case Error:
            styledMessage = QString("<span style='color:#ff4444'>%1</span>").arg(message);
            break;
        case Correct:
            styledMessage = QString("<span style='color:#00cc66'>%1</span>").arg(message);
            break;
        case ConsoleText:
            styledMessage = corsacPrefix + QString("<span style='color:#ffd633'>%1</span>").arg(message);
            break;
        case UserInput:
            styledMessage = QString("<span style='color:#f0f0f0'>%1</span>").arg(message);
            break;
    }
    // Removed invalid method call as setDragEnabled is not defined for QTextEdit or its parent classes.
    setAcceptDrops(false);
    // Вызываем метод базового класса
    ConsoleDisplay::printMessage(styledMessage);
    moveCursor(QTextCursor::End);
}

void Console::prompt() {
    append(QString("<span style='color:White'>&nbsp;➜</span> "));
    moveCursor(QTextCursor::End);
    inputBuffer.clear();
    promptPosition = textCursor().position();
}

void Console::keyPressEvent(QKeyEvent *event) {
    // Обработка подсказок (стрелки, Tab)

    QTextCursor cursor = textCursor();

    // Обработка Ctrl+C
    if (event->matches(QKeySequence::Copy)) {
        if (cursor.hasSelection()) {
            QApplication::clipboard()->setText(toPlainText());
            event->accept();
        }
        return;
    }

    if (event->matches(QKeySequence::Paste)) {
        QString clipboardText = QApplication::clipboard()->text(); // Получаем текст из буфера обмена

        // Проверяем, находится ли выделение за пределами редактируемой области
        if (cursor.hasSelection()) {
            int selStart = cursor.selectionStart();
            int selEnd = cursor.selectionEnd();

            // Если выделение затрагивает область вне редактируемой зоны
            if (selStart < promptPosition) {
                // Ограничиваем выделение только редактируемой областью
                cursor.setPosition(qMax(selStart, promptPosition), QTextCursor::MoveAnchor);
                cursor.setPosition(selEnd, QTextCursor::KeepAnchor);
                setTextCursor(cursor);
            }
        }

        // Убедимся, что вставка происходит в редактируемой области
        if (cursor.position() < promptPosition) {
            moveCursor(QTextCursor::End);
        }

        cursor.insertText(clipboardText); // Вставляем текст без форматирования
        setTextCursor(cursor);
        event->accept();
        return;
    }

    // Коррекция ввода: если ввод вне редактируемой области, перемещаем курсор
    if (!event->text().isEmpty()) {
        bool hasSelection = cursor.hasSelection();
        int selStart = cursor.selectionStart();
        int selEnd = cursor.selectionEnd();
        if (hasSelection) {
            if (selStart < promptPosition) {
                if (selStart == 0 && selEnd == document()->characterCount() - 1) {
                    cursor.setPosition(promptPosition, QTextCursor::MoveAnchor);
                    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
                    setTextCursor(cursor);
                } else {
                    int newStart = qMax(selStart, promptPosition);
                    if (newStart >= selEnd) {
                        if (cursor.position() >= promptPosition) {
                            cursor.clearSelection();
                            setTextCursor(cursor);
                        } else {
                            event->ignore();
                            return;
                        }
                    } else {
                        cursor.setPosition(newStart, QTextCursor::MoveAnchor);
                        cursor.setPosition(selEnd, QTextCursor::KeepAnchor);
                        setTextCursor(cursor);
                    }
                }
            }
        } else if (cursor.position() < promptPosition) {
            moveCursor(QTextCursor::End);
        }
    }

    // Обработка Backspace и Delete
    if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
        bool hasSelection = cursor.hasSelection();
        int selStart = cursor.selectionStart();
        int selEnd = cursor.selectionEnd();
        if (hasSelection) {
            if (selStart < promptPosition) {
                int newStart = qMax(selStart, promptPosition);
                if (newStart >= selEnd) {
                    event->ignore();
                    return;
                }
                cursor.setPosition(newStart, QTextCursor::MoveAnchor);
                cursor.setPosition(selEnd, QTextCursor::KeepAnchor);
                setTextCursor(cursor);
            }
        } else {
            if ((event->key() == Qt::Key_Backspace && cursor.position() <= promptPosition) ||
                (event->key() == Qt::Key_Delete && cursor.position() < promptPosition)) {
                event->ignore();
                return;
            }
        }
        QTextEdit::keyPressEvent(event);
        return;
    }

    // Если клавиша для ввода текста, проверяем позицию курсора
    if (!event->text().isEmpty()) {
        if (cursor.position() < promptPosition) {
            event->ignore();
            return;
        }
    }

    // Обработка Enter
    if (event->key() == Qt::Key_Return) {
        QTextCursor cursor = textCursor();
        inputBuffer = cursor.block().text().mid(promptPosition - cursor.block().position()).trimmed();
        m_suggestions->hideSuggestions();
        if (!inputBuffer.isEmpty()) {
            QString response = inputHandler->processCommand(inputBuffer);
            if (inputBuffer == "clear") {
                clear();
                prompt();
            } else {
                // Вывод ответа: если команда echo, считаем ответом ввод пользователя, иначе – консольный вывод
                printMessage(response, inputBuffer.startsWith("echo ") ? UserInput : ConsoleText);
                prompt();
            }
            // Логирование
            logManager->log(inputBuffer);
        } else {
            cursor.setPosition(promptPosition);
            cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
            moveCursor(QTextCursor::End);
        }
        return;
    }

    switch(event->key()) {
        case Qt::Key_Tab:
            acceptSuggestion();
            event->accept();
            return;
    }

    if (event->key() == Qt::Key_Up) {
        if (m_suggestionWidget->isVisible()) {
            if (m_suggestions->suggestionsWidget()->currentRow() == 0) {
                // Если выделение на первом элементе, перемещаем курсор вверх
                QTextEdit::keyPressEvent(event);
            } else {
                // Иначе перемещаемся по подсказкам
                m_suggestions->moveSelection(-1);
                event->accept();
            }
        } else {
            QTextEdit::keyPressEvent(event); // Перемещение курсора вверх
        }
        return;
    }

    if (event->key() == Qt::Key_Down) {
        QTextCursor cursor = textCursor();
    
        // Проверяем, находится ли курсор в строке ввода
        if (cursor.position() >= promptPosition) {
            if (m_suggestionWidget->isVisible()) {
                // Если виджет подсказок активен, перемещаем выделение вниз
                m_suggestions->moveSelection(1);
                event->accept();
            } else {
                // Если виджет подсказок не активен, передаём событие базовому классу
                QTextEdit::keyPressEvent(event);
            }
        } else {
            // Если курсор находится выше строки ввода, перемещаем его вниз
            QTextEdit::keyPressEvent(event);
        }
        return;
    }

    QTextEdit::keyPressEvent(event);
    updateSuggestions();
    updateGhostText();

}

void Console::updateSuggestions() {
    QString textAfterPrompt = toPlainText().mid(promptPosition).trimmed(); // Текст после позиции промпта

    // Если текст после промпта пустой, скрываем виджет подсказок
    if (textAfterPrompt.isEmpty()) {
        m_suggestions->hideSuggestions();
        return;
    }

    QString textBeforeCursor = toPlainText().left(promptPosition); // Текст до позиции промпта
    QChar lastChar = textBeforeCursor.isEmpty() ? QChar() : textBeforeCursor.back(); // Последний символ перед промптом

    // Проверяем предыдущий символ перед текущей позицией
    int cursorPos = textCursor().position();
    if (cursorPos > 0) {
        QChar prevChar = toPlainText().at(cursorPos - 1);
        if (prevChar.isSpace()) {
            m_suggestions->hideSuggestions();
            return;
        }
    }

    if (cursorPos < promptPosition) {
        m_suggestions->hideSuggestions();
        return;
    }

    QString prefix = m_suggestions->currentPrefix(toPlainText(), promptPosition).trimmed(); // Убираем пробелы

    // Если префикс пустой, скрываем виджет подсказок
    if (prefix.isEmpty()) {
        m_suggestions->hideSuggestions();
        return;
    }

    QStringList commands = inputHandler->availableCommands();

    // Обновляем подсказки
    m_suggestions->updateSuggestions(prefix, commands);

    // Если нет подходящих команд, скрываем виджет подсказок
    if (m_suggestionWidget->count() == 0) {
        m_suggestions->hideSuggestions();
        return;
    }

    // Показываем подсказки
    m_suggestions->showSuggestions(cursorRect(), this);
}

// Принятие подсказки
void Console::acceptSuggestion() {
    QListWidgetItem *current = m_suggestionWidget->currentItem();
    if (current) {
        QString selected = current->text();
        QString prefix = m_suggestions->currentPrefix(toPlainText(), promptPosition);

        QTextCursor cursor = textCursor();
        cursor.setPosition(promptPosition);
        cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
        cursor.removeSelectedText(); // удаляем всё после промпта
        cursor.insertText(selected); // вставляем выбранное
        setTextCursor(cursor);
        
        m_ghostText.clear();
        m_suggestions->hideSuggestions();
    }
}

// Отрисовка призрачного текста
void Console::paintEvent(QPaintEvent *event) {
    QTextEdit::paintEvent(event);
    
    if(!m_ghostText.isEmpty()) {
        QPainter painter(viewport());
        painter.setPen(QColor(100, 100, 100));
        
        QTextCursor tc = textCursor();
        QRect rect = cursorRect(tc);
        painter.drawText(rect.translated(2, 0), Qt::AlignLeft | Qt::AlignVCenter, m_ghostText);
    }
}

// Обновление призрачного текста
void Console::updateGhostText() {
    QString prefix = m_suggestions->currentPrefix(toPlainText(), promptPosition);
    QStringList commands = inputHandler->availableCommands();
    
    m_ghostText.clear();
    for(const QString &cmd : commands) {
        if(cmd.startsWith(prefix) && cmd != prefix) {
            m_ghostText = cmd.mid(prefix.length());
            break;
        }
    }
}


