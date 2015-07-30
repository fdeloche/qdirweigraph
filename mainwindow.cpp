#include <QtGui>

#include "mainwindow.h"
#include <QDebug>
#include <QFile>
#include "noeud.h"
#include "graphscale.h"

#include <QInputDialog>
#include <QFileDialog>
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QMenuBar>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QComboBox>
#include <QCheckBox>

//temp
#include "graphexml.h"
#include "tinyxml.h"

#include <QSvgGenerator>

#include <QTextStream>

#include <cmath>

#include <QGridLayout>
#include <QStatusBar>

#include <locale>

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

    //QString message = tr("A context menu is available by right-clicking");
    //statusBar()->showMessage(message);

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
mainFolder = filename.section('/', 0, -2);
    if(!graph)
        unlockActions();

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
    mainFolder = filename.section('/', 0, -2);
    saveFile(filename);


}

void MainWindow::saveAsSvg(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save as svg"),
                                                   mainFolder,
                                                    tr("SVG files (*.svg)"));
    if(svgOptions())
      saveSvg(filename);
}

void MainWindow::saveSvg(QString filename){
    if(graph){
    QSvgGenerator generator;
        generator.setFileName(filename);
        generator.setSize(QSize(svgWidth, svgHeight));
        generator.setViewBox(QRect(0, 0,svgWidth, svgHeight));
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
    saveAct->setEnabled(false);

    modifyScale = new QAction(tr("&Change scale"), this);
    modifyScale->setStatusTip(tr("Change scale"));
    connect(modifyScale, SIGNAL(triggered()), this, SLOT(changeScale()));
    modifyScale->setEnabled(false);

    saveSvgAct = new QAction(tr("&Export current graph as svg"), this);
    saveSvgAct->setStatusTip(tr("Export as SVG"));
    connect(saveSvgAct, SIGNAL(triggered()), this, SLOT(saveAsSvg()));
    saveSvgAct->setEnabled(false);

    openAndSaveSvgAct = new QAction(tr("&Export multiple graphs as SVG"), this);
    openAndSaveSvgAct->setStatusTip((tr("Open selected graphs and export them as svg")));
    connect(openAndSaveSvgAct, SIGNAL(triggered()), this, SLOT(openAndSaveSvg()));

    newGraphAct = new QAction(tr("&New graph"), this);
    newGraphAct->setShortcuts(QKeySequence::New);
    newGraphAct->setStatusTip(tr("Create a new graph"));
    connect(newGraphAct, SIGNAL(triggered()), this, SLOT(newGraph()));


    addArrowAct = new QAction(tr("&Add arrow"), this);
    addArrowAct->setShortcuts(QKeySequence::SelectAll); //Ctrl + A
    addArrowAct ->setStatusTip(tr("Add a new arrow or set value for an existing arrow"));
    connect(addArrowAct, SIGNAL(triggered()), this, SLOT(addArrow()));
    addArrowAct->setEnabled(false);

    setLabelAct = new QAction(tr("&Add labels"), this);
    setLabelAct->setStatusTip(tr("Add labels for nodes"));
    connect(setLabelAct, SIGNAL(triggered()), this, SLOT(setLabels()));
    setLabelAct->setEnabled(false);

    setThresholdAct = new QAction(tr("&Add a threshold"), this);
    connect(setThresholdAct, SIGNAL(triggered()), this, SLOT(setThreshold()));
    setThresholdAct->setEnabled(false);


    importTemplateAct = new QAction(tr("&Import template"), this);
    importTemplateAct->setStatusTip(tr("Import nodes position from another graph"));
    connect(importTemplateAct, SIGNAL(triggered()), this, SLOT(importTemplate()));
    importTemplateAct->setEnabled(false);

    displayAct = new QAction(tr("&Display"), this);
    displayAct->setStatusTip("& Configure display");
    connect(displayAct, SIGNAL(triggered()), this, SLOT(changeDisplay() ));

    changeTitleAct = new QAction(tr("Change title"), this);
    connect(changeTitleAct, SIGNAL(triggered()), this, SLOT(changeTitle()) );
    changeTitleAct->setEnabled(false);
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
    editMenu->addAction(setLabelAct);
    editMenu->addAction(importTemplateAct);
    editMenu->addAction(changeTitleAct);

    displayMenu = menuBar()->addMenu(tr("&Display"));
    displayMenu->addAction(displayAct);


}


void MainWindow::unlockActions(){
        saveAct->setEnabled(true);
        modifyScale->setEnabled(true);
        saveSvgAct->setEnabled(true);
        addArrowAct->setEnabled(true);
        setLabelAct->setEnabled(true);
        setThresholdAct->setEnabled(true);
        importTemplateAct->setEnabled(true);
        changeTitleAct->setEnabled(true);
}

void MainWindow::changeTitle(){
    QString title = QInputDialog::getText(this, tr("Set Title"), tr("Title : "), QLineEdit::Normal, graph->getTitle());
    graph->setTitle(title);
    this->update();
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
    if(graph){
    graph->setOptions(options);
    gscale->setOptions(options);
    dwid->update();
    gscale->update();
    }
}

void MainWindow::importTemplate(){

    if(graph){
        QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        mainFolder,
                                                        tr("XML files (*.xml)"));
        mainFolder = filename.section('/', 0, -2);

        if(!filename.isNull()){
            graph->importTemplate(filename);
            this->update();
        }
    }
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

void MainWindow::setLabels(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Open labels file"),
                                                                  mainFolder,
                                                                  tr("Text files (*.txt)"));
    mainFolder = filename.section('/', 0, -2);
    if(!filename.isNull()){
        this->graph->importLabels(filename);
        this->update();
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
    mainFolder = sList[0].section('/', 0, -2);
    QString filename = QFileDialog::getSaveFileName(this, tr("Save report"),
                                                    mainFolder,
                                                    tr("HTML files (*.html)"));
    mainFolder = filename.section('/', 0, -2);
    QString folder = mainFolder;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        std::cerr << "Cannot open file for writing: "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }

    QString title = QInputDialog::getText(this,"Set title", "Title : ");

    if(svgOptions()){
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
}

void MainWindow::newGraph(){
    int n = QInputDialog::getInt(this, "Number of nodes", "n : ", 1, 0, 200);

    if(!graph)
        unlockActions();

    //if *graph exists we delete it
    delete graph;

    graph = new Graphe(n);
    dwid->setGraph(graph);
    gscale->setMaxvalue(graph->getMaxAdj());

    dwid->update();



}

void MainWindow::addArrow(){
    if(!graph){
        QMessageBox msgBox;
        msgBox.setText("There is no graph");
        msgBox.exec();
        return;
    }
    int ni, nj;
    float value;
    QDialog dialog(this);
    dialog.setWindowTitle("Add an arrow");
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel("Add an arc i->j "));


    QLineEdit * lineEditI = new QLineEdit(&dialog);
    lineEditI->setValidator(new QIntValidator(0, graph->getn()-1, lineEditI));
    form.addRow("i : ", lineEditI);

    QLineEdit * lineEditJ= new QLineEdit(&dialog);
    lineEditJ->setValidator(new QIntValidator(0, graph->getn()-1, lineEditJ));
    form.addRow("j : ", lineEditJ);


    QLineEdit * lineEditV= new QLineEdit(&dialog);
    QDoubleValidator * qdv = new QDoubleValidator(0., 1000., 10, lineEditV);
    qdv->setLocale(QLocale::C);
    lineEditV->setValidator(qdv);
    form.addRow("value (use dot) : ", lineEditV);

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
        value = lineEditV->text().toFloat();
        graph->addArrow(ni, nj, value);
        dwid->update();
        gscale->setMaxvalue(graph->getMaxAdj());
    }


}


bool MainWindow::svgOptions(){

    QDialog dialog(this);
    dialog.setWindowTitle("Svg options");
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    QLineEdit * lineEditW = new QLineEdit(&dialog);
    lineEditW->setText(QString::number(svgWidth));
    lineEditW->setValidator(new QIntValidator(0, 2000, this));
    form.addRow("width : ", lineEditW);

    QLineEdit * lineEditH= new QLineEdit(&dialog);
    lineEditH->setText(QString::number(svgHeight));
    lineEditH->setValidator(new QIntValidator(0, 2000, this));
    form.addRow("height : ", lineEditH);

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        // If the user didn't dismiss the dialog, do something with the fields

        svgWidth = lineEditW->text().toInt();
        svgHeight = lineEditH->text().toInt();

    }
    return QDialog::Accepted;


}

void MainWindow::changeDisplay(){


    QDialog dialog(this);
    dialog.setWindowTitle("Change display");

    QFormLayout form(&dialog);

    QComboBox * styleBox = new QComboBox(&dialog);

    styleBox->addItem(tr("Blue - Purple"));
    styleBox->addItem(tr("Green - Red"));
    styleBox->addItem(tr("Black"));

    styleBox->setCurrentIndex(options.style);

    form.addRow("Style : ", styleBox);

    QCheckBox * displayNode = new QCheckBox("", &dialog);
    displayNode->setChecked(options.displayLabels);

    form.addRow("Display labels : ", displayNode);


    QCheckBox * displayTitle = new QCheckBox("", &dialog);
    displayTitle->setChecked(options.displayTitle);

    form.addRow("Display title : ", displayTitle);

    QSlider * curveSlider = new QSlider(Qt::Horizontal,&dialog);
    curveSlider->setMinimum(0);
    curveSlider->setMaximum(120);
    curveSlider->setValue(options.curve);

    form.addRow("Curve : ", curveSlider);

    QSlider * thicknessSlider = new QSlider(Qt::Horizontal,&dialog);
    thicknessSlider->setMinimum(5);
    thicknessSlider->setMaximum(80);
    thicknessSlider->setValue(options.thick);

    form.addRow("Thickness : ", thicknessSlider);

    QSlider * dSlider = new QSlider(Qt::Horizontal,&dialog);
    dSlider->setTickInterval(1);
    dSlider->setRange(5, 30);
    dSlider->setValue(options.maxarcs);

    form.addRow("Arrows per nodes : ", dSlider);


    QCheckBox * fillArrows = new QCheckBox("", &dialog);
    fillArrows->setChecked(options.fill);

    form.addRow("Fill arrows : ", fillArrows);

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));


    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        qDebug()<< styleBox->currentIndex();
        options.style = styleBox->currentIndex();
        options.displayLabels= displayNode->isChecked();
        options.displayTitle=displayTitle->isChecked();
        options.curve = curveSlider->value();
        options.thick = thicknessSlider->value();
        options.maxarcs = dSlider->value();
        options.fill = fillArrows->isChecked();
        // If the user didn't dismiss the dialog, do something with the fields
        this->update();
}


}

MainWindow::~MainWindow(){
    delete graph;
}
