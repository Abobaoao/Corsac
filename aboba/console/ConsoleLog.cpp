#include "ConsoleLog.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

ConsoleLog::ConsoleLog(const QString &logFileName)
    : m_logFileName(logFileName)
{
}

void ConsoleLog::log(const QString &message) {
    QFile file(m_logFileName);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << message << "\n";
        file.close();
    } else {
        qWarning() << "Unable to open log file:" << m_logFileName;
    }
}
