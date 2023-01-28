#include "Command.h"

//构造函数
Command::Command() : Left(), Right(), Path() { }
Command::Command(QString left, QString right, QString load)
    : Left(left), Right(right), Path(load) { }
Command::Command(const Command& other)
    : Left(other.Left), Right(other.Right), Path(other.Path) { }
const Command& Command::operator=(const Command& other) {
    this->Path = other.Path;
    this->Left = other.Left;
    this->Right = other.Right;
    return *this;
}
//析构函数
Command::~Command() {}

QDebug operator<<(QDebug dbg, const Command& cmd) {
    dbg << "(" + cmd.Left + "," + cmd.Right + "," + cmd.Path + ")";
    return dbg;
}
QDataStream& operator<<(QDataStream& qds, const Command& cmd) {
    qds << cmd.Left << cmd.Right << cmd.Path;
    return qds;
}
QDataStream& operator>>(QDataStream& qds, Command& cmd) {
    qds >> cmd.Left >> cmd.Right >> cmd.Path;
    return qds;
}