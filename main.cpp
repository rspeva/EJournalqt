#include "userwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    w.show();

    return a.exec();
}
