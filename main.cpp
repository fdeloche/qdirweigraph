
#include <QApplication>

#include "mainwindow.h"

#include <QDebug>

#include <stdlib.h>

//TODO
//modes
//display title
// curve

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
