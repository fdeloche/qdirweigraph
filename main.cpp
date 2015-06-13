
#include <QApplication>

#include "mainwindow.h"

#include <QDebug>

#include <stdlib.h>

//TODO
//modes
//able/disable actions
//display title nodes

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
