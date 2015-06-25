
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawwidget.h"
#include "graphe.h"
#include <QString>

#include "graphscale.h"

#include "helpwidget.h"
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
    void showHelp();
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
    QMenu *helpMenu;

    QAction *openAct;
    QAction * newGraphAct;
    QAction * saveAct;
    QAction * addArrowAct;
    QAction * modifyScale;
    QAction * saveSvgAct;
    QAction * openAndSaveSvgAct;
    QAction * setThresholdAct;
    QAction * setLabelAct;
    QAction * showHelpAct;
    QAction * importTemplateAct;
    QAction * displayAct;
    QAction * changeTitleAct;

    DrawWidget * dwid;
    HelpWidget * helpWid;
    GraphScale * gscale;
    Graphe * graph;

    //svg export
    int svgWidth = 600;
    int svgHeight = 600;

    //QLabel *infoLabel;
    graphOptions options;


};

#endif
