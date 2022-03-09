#include "udp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UDP w;
    w.show();
    return a.exec();
}
