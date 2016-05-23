#include "pqOutlierFilterListWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pqOutlierFilterListWidget w;
    w.show();

    return a.exec();
}
