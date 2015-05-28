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

#include <cmath>

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
                                                    mainFolder,
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

    newGraphAct = new QAction(tr("&New graph"), this);
    newGraphAct->setShortcuts(QKeySequence::New);
    newGraphAct->setStatusTip(tr("Create a new graph"));
    connect(newGraphAct, SIGNAL(triggered()), this, SLOT(newGraph()));

    addArrowAct = new QAction(tr("&Add arrow"), this);
    addArrowAct->setShortcuts(QKeySequence::SelectAll); //Ctrl + A
    addArrowAct ->setStatusTip(tr("Add a new arrow"));
    connect(addArrowAct, SIGNAL(triggered()), this, SLOT(addArrow()));

    setThresholdAct = new QAction(tr("&Add a threshold"), this);
    connect(setThresholdAct, SIGNAL(triggered()), this, SLOT(setThreshold()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newGraphAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveSvgAct);
    fileMenu->addAction(openAndSaveSvgAct);


    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu ->addAction(modifyScale);
    editMenu->addAction(addArrowAct);
    editMenu->addAction(setThresholdAct);
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

void MainWindow::setThreshold(){
    bool ok;
    if(graph){
        float s = QInputDialog::getDouble(this, "Set a threshold value", "Threshold value : ", graph->getThreshold(), 0., 1000., 2, &ok);
        graph->setThreshold(s);
        dwid->update();
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
    if(graph->getp()>1){
        QString folder = filename.section('/', 0, -2)+"/";
        graph->saveA(folder);
    }
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

void MainWindow::newGraph(){
    int n = QInputDialog::getInt(this, "Number of nodes", "n : ", 1, 0, 200);
    int p = QInputDialog::getInt(this, "Length of A(i,j) p - Arcs' weights are given by |A(i,j)| : ", "p : ", 1, 0, 50);

    //if *graph exists we delete it
    delete graph;

    graph = new Graphe(n, p);
    dwid->setGraph(graph);
    gscale->setMaxvalue(graph->getMaxAdj());

}

void MainWindow::addArrow(){
    int ni, nj;
    float value;
    QDialog dialog(this);
    dialog.setWindowTitle("Add an arrow");
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel("Add an arc i->j "));


    QLineEdit * lineEditI = new QLineEdit(&dialog);
    form.addRow("i : ", lineEditI);

    QLineEdit * lineEditJ= new QLineEdit(&dialog);
    form.addRow("j : ", lineEditJ);

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        // If the user didn't dismiss the dialog, do something with the fields

        ni = lineEditI->text().toInt();
        nj = lineEditJ->text().toInt();
    }


    ////// VALUES

    QDialog dialog2(this);
    dialog2.setWindowTitle("Give value(s)");

    QFormLayout form2(&dialog2);
    // Add some text above the fields
    form2.addRow(new QLabel("Values "));

    // Add the lineEdits with their respective labels
    QList<QLineEdit *> fields;
    for(int i = 0; i < graph->getp(); ++i) {
        QLineEdit *lineEdit = new QLineEdit(&dialog);
        QString label = QString("k = %1").arg(i + 1);
        form2.addRow(label, lineEdit);

        fields << lineEdit;
    }

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox2(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form2.addRow(&buttonBox2);
    QObject::connect(&buttonBox2, SIGNAL(accepted()), &dialog2, SLOT(accept()));
    QObject::connect(&buttonBox2, SIGNAL(rejected()), &dialog2, SLOT(reject()));

    // Show the dialog as modal
    value = 0;
    float value2;
    if (dialog2.exec() == QDialog::Accepted) {
        // If the user didn't dismiss the dialog, do something with the fields
        int k=0;
        foreach(QLineEdit * lineEdit, fields) {
            value2 = lineEdit->text().toFloat();
            if(graph->getp()>0)
                graph->setA(nj, ni, k, value2);
            value += value2*value2;
            k++;
        }
    }
    value = sqrt(value);
    graph->addArrow(ni, nj, value);
    dwid->update();
    gscale->setMaxvalue(graph->getMaxAdj());

}

MainWindow::~MainWindow(){
    delete graph;
}
