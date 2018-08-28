#include "mainwindow.h"
#include <QApplication>
#include "cailwindow.h"
#include "accwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if 0
    MainWindow w;
    w.show();
#else
    //CailWindow w;
    AccWindow w;
    w.show();
#endif

    return a.exec();
}
