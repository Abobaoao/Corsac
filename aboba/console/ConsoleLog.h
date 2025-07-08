#ifndef CONSOLELOG_H
#define CONSOLELOG_H

#include <QString>

class ConsoleLog {
public:
    ConsoleLog(const QString &logFileName);
    void log(const QString &message);

private:
    QString m_logFileName;
};

#endif // CONSOLELOG_H
