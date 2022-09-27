#include "qtmulticameras.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtMultiCameras w;
    w.show();
    return a.exec();
}
