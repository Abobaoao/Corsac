#include "CommandSuggestion.h"
#include <QPoint>
#include <QWidget>
#include <QDebug>
#include <algorithm> // Убедитесь, что этот заголовок подключен для std::min



CommandSuggestion::CommandSuggestion(QListWidget *widget)
    : m_widget(widget), m_selectedSuggestion(-1) 
{
    m_widget->setItemDelegate(new CleanItemDelegate(m_widget));
    m_widget->setSpacing(0);
    m_widget->setFocusPolicy(Qt::NoFocus);
    m_widget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_widget->setAttribute(Qt::WA_TranslucentBackground);
    m_widget->setContentsMargins(0, 0, 0, 0);
    m_widget->setWindowFlags(Qt::SubWindow); // Изменяем флаг на дочернее окно
    m_widget->setParent(nullptr);
    
    
    m_widget->setStyleSheet(
        "QListWidget {"
        "   background: transparent;"
        "   border: none;"
        "}"
    );
}

void CommandSuggestion::updateSuggestions(const QString &prefix, const QStringList &commands) {
    QStringList filtered;
    for (const QString &cmd : commands) {
        if (cmd.startsWith(prefix, Qt::CaseInsensitive))
            filtered << cmd;
    }

    // Сортируем по алфавиту
    std::sort(filtered.begin(), filtered.end(), [](const QString &a, const QString &b) {
        return a.toLower() < b.toLower();
    });

    m_widget->clear();
    m_widget->addItems(filtered);
}


void CommandSuggestion::moveSelection(int offset) {
    int count = m_widget->count();
    if (count == 0) return;
    m_selectedSuggestion = std::clamp(m_selectedSuggestion + offset, 0, count - 1);
    m_widget->setCurrentRow(m_selectedSuggestion);
}

void CommandSuggestion::hideSuggestions() {
    m_widget->hide();
    m_selectedSuggestion = -1;
}

QString CommandSuggestion::currentPrefix(const QString &fullText, int promptPosition) {
    return fullText.mid(promptPosition).trimmed();
}

void CommandSuggestion::updateGhostText(const QString &prefix, const QStringList &commands, QString &ghostTextOut) {
    ghostTextOut.clear();
    QStringList filteredCommands;
    for (const QString &cmd : commands) {
        if (cmd.startsWith(prefix)) {
            filteredCommands.append(cmd);
        }
    }

    for (const QString &cmd : filteredCommands) {
        QListWidgetItem *item = new QListWidgetItem(cmd);
        item->setTextAlignment(Qt::AlignVCenter);  // Выравнивание по вертикали
        m_widget->addItem(item);
    }
}

void CommandSuggestion::showSuggestions(const QRect &cursorRect, QWidget *parent) {
    if (!parent || m_widget->count() == 0) {
        hideSuggestions();
        return;
    }

    if (m_widget->parentWidget() != parent) {
        m_widget->setParent(parent);
    }

    m_widget->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint);

    int fixedX = 38; // можно подогнать тут пениса в рот 
    int offsetY = 20;
    QPoint localPos(fixedX, cursorRect.bottom() + offsetY);
    m_widget->move(localPos);

    m_widget->setFixedSize(250, std::min(m_widget->sizeHintForRow(0) * std::min(m_widget->count(), 10) + 2, 250));
    m_widget->setCurrentRow(0); 
    m_widget->raise();
    m_widget->show();
}


QListWidget* CommandSuggestion::suggestionsWidget() const {
    return m_widget;
}
