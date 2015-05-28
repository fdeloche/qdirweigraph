#include <QtGui>

#include "mainwindow.h"
#include <QDebug>
#include <QFile>
#include "noeud.h"
#include "graphscale.h"

#include <QInputDialog>

//temp
#include "graphexml.h"
#include "tinyxml.h"

#include <QSvgGenerator>

#include <QTextStream>
#include <QFile>

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


    gscale = new GraphScale();
    gscale->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    gscale->setStyleSheet("background-color:white");

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    //bottomFiller->setMaximumHeight(50);

    //QSlider *slider = new QSlider(Qt::Horizontal, this);
    //slider->setMinimum(0);
    //slider->setMaximum(80);
    //connect(slider, SIGNAL(valueChanged(int)), dwid, SLOT(changeCurve(int)));


    //slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    QGridLayout *layout = new QGridLayout;
    layout->setMargin(2);
    layout->addWidget(topFiller, 0, 0, 1, 2);
    layout->addWidget(gscale, 1, 0);
    layout->addWidget(dwid, 1, 1);
    layout->addWidget(bottomFiller, 2, 0, 1, 2);
    //layout->addWidget(slider, 3, 0, 1, 2);
    //layout->setStretch(1, 2);
    layout->setColumnMinimumWidth(0, 60);
    //layout->setRowMinimumHeight(3, 15);
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
                                                                  mainFolder,
                                                                  tr("XML files (*.xml)"));
    if(!filename.isNull()){
        this->openFile(filename);
        this->update();
    }
    //infoLabel->setText(tr("Invoked <b>File|Open</b>"));
}

void MainWindow::saveAs(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save file"),
                                                    "./Tests",
                                                    tr("XML files (*.xml)"));
    saveFile(filename);
}

void MainWindow::saveAsSvg(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save as svg"),
                                                   mainFolder,
                                                    tr("SVG files (*.svg)"));
    saveSvg(filename);
}

void MainWindow::saveSvg(QString filename){
    if(graph){
    QSvgGenerator generator;
        generator.setFileName(filename);
        generator.setSize(QSize(600, 600));
        generator.setViewBox(QRect(0, 0, 600, 600));
        generator.setTitle(tr("Graphe"));
        generator.setDescription(tr("An SVG drawing created by the Qt SVG Generator "));

        QPainter painter;
        painter.begin(&generator);
        graph->draw(&painter);
        gscale->paint(&painter);
        painter.end();
    }
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

    modifyScale = new QAction(tr("&Change scale"), this);
    modifyScale->setStatusTip(tr("Change scale"));
    connect(modifyScale, SIGNAL(triggered()), this, SLOT(changeScale()));

    saveSvgAct = new QAction(tr("&Export current graph as svg"), this);
    saveSvgAct->setStatusTip(tr("Export as SVG"));
    connect(saveSvgAct, SIGNAL(triggered()), this, SLOT(saveAsSvg()));

    openAndSaveSvgAct = new QAction(tr("&Export multiple graphs as SVG"), this);
    openAndSaveSvgAct->setStatusTip((tr("Open selected graphs and export them as svg")));
    connect(openAndSaveSvgAct, SIGNAL(triggered()), this, SLOT(openAndSaveSvg()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveSvgAct);
    fileMenu->addAction(openAndSaveSvgAct);


    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu ->addAction(modifyScale);


}

void MainWindow::openFile(QString filename){
    //if *graph exists we delete it
    delete graph;

    graph = new Graphe(filename);
    dwid->setGraph(graph);
    gscale->setMaxvalue(graph->getMaxAdj());

    //DrawWidget dwid(gr);
    //dwid.show();



    //int rep = qapp.exec();

}

void MainWindow::update(){
    dwid->update();
    gscale->update();
}

void MainWindow::changeScale(){
    bool ok;
    if(graph){
        double s = QInputDialog::getDouble(this, "Set value", "Scale : [ 0  - ... : ", gscale->getMaxvalue(), 0, 1000, 2, &ok);
        gscale->setMaxvalue((float) s);
        graph->setMaxAdj((float) s);
        dwid->update();
        gscale ->update();
    }
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

void MainWindow::openAndSaveSvg(){
    QStringList sList = QFileDialog::getOpenFileNames(this, tr("Open files"), mainFolder,
                                                        tr("XML files (*.xml)"));
    QString folder = sList[0].section('/', 0, -2);
    QString filename = QFileDialog::getSaveFileName(this, tr("Save report"),
                                                    folder,
                                                    tr("HTML files (*.html)"));
    folder = filename.section('/', 0, -2);

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        std::cerr << "Cannot open file for writing: "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }

    QString title = QInputDialog::getText(this,"Set title", "Title : ");

    QTextStream out(&file);
    out << "<!DOCTYPE html>" << endl << "<html><body><h1>";
    out << title << "</h1>" << endl;

    for (int i = 0; i < sList.size(); ++i){
         openFile(sList[i]);
         sList[i].replace(".xml", "");
         sList[i] = sList[i].section('/', -1);
         saveSvg(folder + "/" + sList[i] +".svg");
         out << "<p><b>" << graph->getTitle() << "</b></p>";
         out << "<embed type='image/svg+xml' src='" << "./" + sList[i] +".svg" << "' />" << endl;
    }

    out << "</body> </html>";
    file.close();

}

MainWindow::~MainWindow(){
    delete graph;
}
