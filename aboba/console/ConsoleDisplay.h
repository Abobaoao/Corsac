
#ifndef CONSOLEDISPLAY_H
#define CONSOLEDISPLAY_H

#include <QTextEdit>

class ConsoleDisplay : public QTextEdit {
    Q_OBJECT
public:
    explicit ConsoleDisplay(QWidget *parent = nullptr);
    void printMessage(const QString &message);
    
protected:
    void initialize();
};

#endif 