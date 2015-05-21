
#include <QApplication>

#include "mainwindow.h"
#include "tinyxml.h"

#include "stdio.h"

#include "graphexml.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
