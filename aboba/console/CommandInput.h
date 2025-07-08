#ifndef COMMANDINPUT_H
#define COMMANDINPUT_H

#include <QString>
#include <QStringList>

class CommandInput {
public:
    CommandInput() = default; // Явное объявление конструктора по умолчанию
    QString processCommand(const QString &command);
    QStringList availableCommands() const;
};

#endif