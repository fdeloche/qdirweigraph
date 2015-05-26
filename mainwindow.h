
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawwidget.h"
#include "graphe.h"
#include <QString>

#include "graphscale.h"

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
    void changeScale();
    void saveAsSvg();
    void openAndSaveSvg();

private:
    void openFile(QString filename);
    void update();
    void saveSvg(QString filename);
    void saveFile(QString filename);
    void createActions();
    void createMenus();


    QMenu *fileMenu;
    QMenu *editMenu;

    QAction *openAct;
    QAction * saveAct;
    QAction * modifyScale;
    QAction * saveSvgAct;
    QAction * openAndSaveSvgAct;

    DrawWidget * dwid;
    GraphScale * gscale;
    Graphe * graph;
    //QLabel *infoLabel;

};

#endif
