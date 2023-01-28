#pragma once
#include<qstring.h>
#include<qdebug.h>
#include<qdatastream.h>

class Command : public QObject {
public:
    Command();
    Command(QString left, QString right, QString load);
    Command(const Command& other);
    const Command& operator=(const Command& other);
    ~Command();
    QString Left, Right;  //cmd: Left + Path + Right
    QString Path;  //file's load
};

QDebug operator<<(QDebug dbg, const Command& cmd);
QDataStream& operator<<(QDataStream& qds, const Command& cmd);
QDataStream& operator>>(QDataStream& qds, Command& cmd);