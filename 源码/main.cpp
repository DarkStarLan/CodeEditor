#include "QtTool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtTool w;
    w.setFocusPolicy(Qt::StrongFocus);
    w.show();
    return a.exec();
}
