#include "pqIcpPropertyWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pqIcpPropertyWidget w;
    w.show();

    return a.exec();
}
