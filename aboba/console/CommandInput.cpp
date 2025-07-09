#include "CommandInput.h"
#include <QStringList>

QString CommandInput::processCommand(const QString &command) {
    if (command == "help") {
        return "Available commands: help, clear, echo [text]";
    } else if (command.startsWith("echo ")) {
        return command.mid(5);
    } else if (command == "clear") {
        return "";  
    } else {
        return "Unknown command";
    }
}
QStringList CommandInput::availableCommands() const {
    return {"help", "clear", "echo", "exit", "error", "eholp"};
}
