
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawwidget.h"
#include "graphe.h"
#include <QString>

#include "graphscale.h"


#include "graphoptions.h"

#include <QIntValidator>

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
    void newGraph();
    void addArrow();
    void setThreshold();
    void setLabels();
    void importTemplate();
    void changeDisplay();
    void changeTitle();

private:
    bool svgOptions();
    void openFile(QString filename);
    void update();
    void saveSvg(QString filename);
    void saveFile(QString filename);
    void createActions();
    void createMenus();
    void unlockActions();

    QString mainFolder = "/home/";

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *displayMenu;

    QAction *openAct;
    QAction * newGraphAct;
    QAction * saveAct;
    QAction * addArrowAct;
    QAction * modifyScale;
    QAction * saveSvgAct;
    QAction * openAndSaveSvgAct;
    QAction * setThresholdAct;
    QAction * setLabelAct;
    QAction * importTemplateAct;
    QAction * displayAct;
    QAction * changeTitleAct;

    DrawWidget * dwid;
    GraphScale * gscale;
    Graphe * graph;

    //svg export
    int svgWidth = 600;
    int svgHeight = 600;

    //QLabel *infoLabel;
    graphOptions options;


};

#endif
