
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawwidget.h"
#include "graphe.h"
#include <QString>

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected:
 //void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void open();
    void saveAs();

private:
    void openFile(QString filename);
    void saveFile(QString filename);
    void createActions();
    void createMenus();


    QMenu *fileMenu;
    QAction *openAct;
    QAction * saveAct;
    DrawWidget * dwid;
    Graphe * graph;
    //QLabel *infoLabel;

};

#endif
