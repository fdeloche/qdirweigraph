#include <QtGui>

#include "mainwindow.h"
#include <QDebug>
#include <QFile>
#include "noeud.h"

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
                                                                  tr("Text files (*.txt)"));
    if(!filename.isNull())
        this->openFile(filename);

    //infoLabel->setText(tr("Invoked <b>File|Open</b>"));
}

void MainWindow::saveAs(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                    "./Tests",
                                                    tr("Text files (*.txt)"));
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
    //QFile inputFile("/home/fdeloche/Desktop/workspaces/Qt/Graphes/test.txt");
    QFile inputFile(filename);
    int n;
    Noeud * noeuds;

    float * * adj;


    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       QString line = in.readLine();
       while (line != "n")
       {
          line = in.readLine();
       }
       line = in.readLine();
       qDebug() << qPrintable(" n : ") << qPrintable(line);
       n = line.toInt();

       noeuds = new Noeud[n];

       while (line != "Noeuds" & line!="Nodes")
       {
          line = in.readLine();
       }
       QStringList coord;
       int x, y;
       int i = 0;
       line = in.readLine();
       while (line !="")
       {
          coord = line.split(";");
          x = coord[0].toInt();
          y = coord[1].toInt();
          noeuds[i] = Noeud(x,y);
          i++;
          line = in.readLine();
       }


       while (line != "Coeffs")
       {
          line = in.readLine();
       }
       adj = new float*[n];

           for(int i=0; i<n; i++){
               adj[i] = new float [n];
               for(int j=0; j<n; j++){
                   adj[i][j]=0.;
               }
           }
       i=0;
       while (!in.atEnd())
       {
          line = in.readLine();
          coord = line.split(";");
          for(int j=0; j<n; j++){
             adj[i][j] = coord[j].toFloat();
             //qDebug() << adj[i][j];
          }
          //qDebug() << endl;
          i++;

       }

       inputFile.close();
    }else{
        qDebug() << qPrintable("Erreur lors de la lecture du fichier texte");
    }

    graph = new Graphe(noeuds,adj, n);
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

    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &file );
        stream << "File to generate a graph" << endl;
        stream << endl;
        stream << "This file was generated automatically" << endl;
        stream << endl;

        stream << "Number of nodes : " << endl;
        stream << endl;

        stream << "n" << endl;
        stream << graph->getn() << endl;

        stream << endl << "Nodes" << endl;
        graph->printNodes(stream);


        stream << endl << "Coeffs" << endl;
        graph->printCoeffs(stream);

    }
    file.close();
}

MainWindow::~MainWindow(){
    delete graph;
}
