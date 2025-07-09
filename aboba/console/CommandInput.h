#ifndef COMMANDINPUT_H
#define COMMANDINPUT_H

#include <QString>
#include <QStringList>

class CommandInput {
public:
    CommandInput() = default; 
    QString processCommand(const QString &command);
    QStringList availableCommands() const;
};

#endif