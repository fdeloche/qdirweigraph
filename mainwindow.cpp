#include <QtGui>

#include "mainwindow.h"
#include <QDebug>
#include <QFile>
#include "noeud.h"

//temp
#include "graphexml.h"
#include "tinyxml.h"

MainWindow::MainWindow()
{
    graph = NULL;
    QWidget * widget = new QWidget();
    setCentralWidget(widget);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    //topFiller->setMaximumHeight(50);
    /*infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                              "invoke a context menu</i>"));
    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    infoLabel->setAlignment(Qt::AlignCenter);
    */

    dwid = new DrawWidget();
    dwid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    dwid->setStyleSheet("background-color:white;");

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    //bottomFiller->setMaximumHeight(50);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(topFiller);
    layout->addWidget(dwid);
    layout->addWidget(bottomFiller);
    //layout->setStretch(1, 2);
    widget->setLayout(layout);

    createActions();
    createMenus();

    QString message = tr("A context menu is available by right-clicking");
    statusBar()->showMessage(message);

    setWindowTitle(tr("Graphe - Visionneur"));
    setMinimumSize(160, 160);
    resize(480, 320);
}

/*
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(cutAct);
    menu.addAction(copyAct);
    menu.addAction(pasteAct);
    menu.exec(event->globalPos());
}*/


void MainWindow::open()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                                  "./Tests/",
                                                                  tr("XML files (*.xml)"));
    if(!filename.isNull())
        this->openFile(filename);

    //infoLabel->setText(tr("Invoked <b>File|Open</b>"));
}

void MainWindow::saveAs(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                    "./Tests",
                                                    tr("XML files (*.xml)"));
    saveFile(filename);
}


void MainWindow::createActions()
{

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open graph file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save as..."), this);
    saveAct->setShortcuts(QKeySequence::SaveAs);
    saveAct->setStatusTip(tr("Save graph as..."));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveAs()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
}

void MainWindow::openFile(QString filename){
    //if *graph exists we delete it
    delete graph;

    graph = new Graphe(filename);
    dwid->setGraph(graph);
    dwid->update();
    //DrawWidget dwid(gr);
    //dwid.show();



    //int rep = qapp.exec();

}

void MainWindow::saveFile(QString filename){
    if(!graph){
        QMessageBox msgBox;
        msgBox.setText("There is no graph to save");
        msgBox.exec();
        return;
    }
    graph->saveGraph(filename);

}

MainWindow::~MainWindow(){
    delete graph;
}
