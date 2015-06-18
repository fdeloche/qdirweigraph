#include <graphe.h>

#include <QString>
#include <QDebug>
#include <cmath>

#include "tinyxml.h"
#include "graphexml.h"

#include <stdlib.h>

#include <QPainterPath>

#include <locale>

#include <cmath>


#include <iostream>
#include <fstream>
#include <string>

#include <QFile>

Graphe::Graphe(){
    n=0;
    maxadj=0;
    title="";

}

Graphe::Graphe(int n, int p){
    this->p=p;
    this->n=n;
    maxadj = 0;
    title="";
   adj = new float*[n];
    for(int i=0; i<n; i++){
        adj[i] = new float[n];
        for(int j=0; j<n; j++){
            adj[i][j] = 0;
        }
    }
    noeuds = new Noeud[n];
    for(int i=0; i<n; i++){
        noeuds[i] = Noeud(50 + 40*cos(2*3.14159/n*i), 50 + 40*sin(2*3.14159/n*i));
    }

    labels = new QString[n];

    //Init
    for(int i=0; i<n; i++)
        labels[i]=QString();

    if(p>1){
        //Define matrix A
        matA = new float**[n];
        for(int i=0; i<n; i++){
            matA[i] = new float*[n];
            for(int j=0; j<n; j++){
                matA[i][j] = new float[p];
                for(int k=0; k<p; k++)
                    matA[i][j][k]=0;
            }
        }
      }
}

void Graphe::findMaxadj(){
    float temp = 0;
    for(int i=0; i<n; i++){
       for(int j=0; j<n; j++){
            if(adj[i][j] > temp)
                temp=adj[i][j];
       }
    }
    maxadj=temp;
}

Graphe::Graphe(Noeud * noeuds, float * * adj, int n):noeuds(noeuds), adj(adj), n(n){
    this->findMaxadj();

    labels = new QString[n];

    //Init
    for(int i=0; i<n; i++)
        labels[i]=QString();
}

Graphe::Graphe(QString &filename){
    n=0;



    int i, j, x, y;
    TiXmlDocument doc(filename.toStdString());
    if(doc.LoadFile()){
        //TiXmlHandle hDoc(&doc);
        TiXmlElement * pGraph;
        pGraph = doc.FirstChildElement("Graph");
        if(pGraph){
            pGraph->QueryIntAttribute("n", &n);
            if(pGraph->Attribute("Title"))
                title = QString(pGraph->Attribute("Title"));
            if(pGraph->Attribute("Color")){
                if(pGraph->Attribute("Color")[0] == 'r')
                    color_r = 200;
                if(pGraph->Attribute("Color")[0] == 'b')
                    color_b = 200;

            }
            noeuds = new Noeud[n];

            labels = new QString[n];

            //Init
            for(int i=0; i<n; i++)
                labels[i]=QString();

            for(int i=0; i<n; i++){
                noeuds[i] = Noeud(50 + 40*cos(2*3.14159/n*i), 50 + 40*sin(2*3.14159/n*i));
            }

            adj = new float*[n];

            for(int i=0; i<n; i++){
                adj[i] = new float [n];
                for(int j=0; j<n; j++){
                    adj[i][j]=0.;
                }
            }
            TiXmlElement * pRoot;
            TiXmlElement * pChildren;
            std::string temps;
            pRoot = pGraph->FirstChildElement("Nodes");
            if(pRoot){
                pChildren = pRoot->FirstChildElement("Node");
                while(pChildren){
                    pChildren->QueryIntAttribute("id", &i);

                    pChildren->QueryIntAttribute("x", &x);
                    pChildren->QueryIntAttribute("y", &y);
                    noeuds[i]= Noeud(x,y);
                    if(pChildren->QueryStringAttribute("Label", &temps) == TIXML_SUCCESS)
                        labels[i]= QString(temps.c_str());
                    pChildren = pChildren->NextSiblingElement("Node");
                }
            }
            pRoot = pGraph->FirstChildElement("Arrows");
            if(pRoot){
                pChildren = pRoot->FirstChildElement("Arrow");
                while(pChildren){
                    pChildren->QueryIntAttribute("From", &i);
                    pChildren->QueryIntAttribute("To", &j);
                    setlocale(LC_NUMERIC, "C");
                    if(pChildren)
                        adj[i][j] = std::atof(pChildren->Attribute("Value"));
                    //qDebug() << i << j << pChildren->Attribute("Value");
                    pChildren = pChildren->NextSiblingElement("Arrow");
                }
            }
            this->findMaxadj();
            pRoot = pGraph ->FirstChildElement("Config");
            if(pRoot){
                pChildren = pRoot;
                setlocale(LC_NUMERIC, "C");
                if(pChildren->Attribute("ScaleMax"))
                    maxadj=std::atof(pChildren->Attribute("ScaleMax"));

            }
        }
    }

}

void Graphe::draw(QPainter * qp){
    float w= qp->window().width()/100.;
    float  h= qp->window().height()/100.;
    //w = std::min(w, h);
    //h = std::min(w,h);   //a faire plus tard
    //qDebug() << w << qPrintable(" ") << h;
    qp->setBrush(Qt::black);
    int x, y;

    int r = 2;
    //TITLE
    if(displayTitle)
        qp->drawText(15, 15, title);
    //NODES
    for(int i =0; i<this->n; i++){
        noeuds[i].reset();
        x = noeuds[i].getx()*w;
        y= noeuds[i].gety()*h;
        //qDebug() << i << noeuds[i].getx() <<  noeuds[i].gety();
        qp->drawEllipse(x-r/2, y-r/2, r, r);
        if(drawLabel){
            if(x<w*50.){
                if(y<h*50.){
                    qp->drawText(x-13, y-5, label(i));
                }else{
                    qp->drawText(x-13, y+13, label(i));
                }
            }else{
                if(y<h*50.){
                    qp->drawText(x+5, y-5, label(i));
                }else{
                    qp->drawText(x+5, y+13, label(i));
                }
            }
        }
    }
    float alpha;
    float thick;
    float ratio;
    for(int i =0; i<this->n; i++){
        for(int j =0; j<this->n; j++){
            if(adj[i][j] > threshold){
                if(maxadj!=0){

                    if(type==GREENRED || type==BLUEPURPLE){
                     //if adj[i][j]<0.5 maxadj, change alpha, >0.6 ou >0.5change color < 1 change color and thickness
                    ratio = adj[i][j]/maxadj;
                    alpha = (ratio<0.8) ? 255*(adj[i][j]/(0.8*maxadj)) : 255;
                    if(type==GREENRED){
                    color_r = (ratio<0.5) ? 0 : 255.*(ratio-0.3)/0.7;
                    color_v = (ratio<0.5) ? 175 : 175 - 175*(ratio-0.3)/0.7;
                    color_b = 0;
                    }
                    if(type==BLUEPURPLE){
                        color_r = (ratio<0.6) ? 40 : 40 + 215.*(ratio-0.5)/0.5;
                        color_v=100;
                        //color_v = (ratio<0.3) ? 175 : 175 - 175*(ratio-0.3)/0.7;
                        color_b = (ratio<0.6) ? 200 : 200 + 55.*(ratio-0.5)/0.5;
                    }
                    thick = (ratio<0.2) ? 0.5 : adj[i][j]/maxadj*2.5;
                    thick = (ratio<0.6) ? thick : adj[i][j]/maxadj*6. - 2.1f;
                    qp->setPen(QPen(QColor(color_r, color_v, color_b, alpha), thick, Qt::SolidLine));
                    qp->setBrush(Qt::NoBrush);
                    }
                    if(type==BLACKGRAPH){
                        color_b = 0;
                        color_r=0;
                        color_v = 0;
                        //if adj[i][j]<0.3 maxadj, change alpha, else change thickness (grosso modo)
                    ratio = adj[i][j]/maxadj;
                    alpha = (ratio<0.3) ? 255*(adj[i][j]/(0.3*maxadj)) : 255;
                    thick = (ratio<0.2) ? 0.5 : adj[i][j]/maxadj*5.;
                    qp->setPen(QPen(QColor(color_r, color_v, color_b, alpha), thick, Qt::SolidLine));
                    qp->setBrush(Qt::NoBrush);
                    }
                    if(i!=j)
                        this->drawArrow(qp, i, j, w, h);
                }
            }
        }
    }
}

Noeud * Graphe::clickNode(int x, int y, int r){
    for(int i=0; i<n; i++){
        //Manhattan distance
        if( abs(noeuds[i].getx()-x) + abs(noeuds[i].gety()-y) <r){
            return &noeuds[i];
        }
    }
    return NULL;
}


Coord Graphe::clickEdge(int x, int y, int r){
    int xa, xb, ya, yb;
    float y2;
    bool b;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(adj[i][j]!=0. & (i!=j)){
                xa = noeuds[i].getx();
                ya = noeuds[i].gety();
                xb = noeuds[j].getx();
                yb = noeuds[j].gety();
                b = (x>xa) && (x>xb) || (y>ya) && (y>yb) || (x<xa) && (x<xb) || (y<ya) && (y<yb);
                if(!b){
                    if(xa == xb)
                        return Coord(i, j);
                    y2 = (yb - ya)/(float)(xb- xa)*(x-xa) + ya;
                    //Manhattan distance
                    if( abs((float) (y-y2) ) <r){
                        return Coord(i, j);
                    }
                }
            }
        }
    }
    return Coord(-1, -1);
}


int Graphe::getn(){
    return n;
}

void Graphe::printNodes(QTextStream& stream){
    if (n==0)
            return;
    for(int i=0; i<n; i++){
         stream << noeuds[i].getx() << "; " << noeuds[i].gety() <<endl;
    }
}

void Graphe::printCoeffs(QTextStream& stream){
    if (n==0)
            return;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            stream << adj[i][j] << "; ";
        }
     stream << endl;
    }
}

float Graphe::valueEdge(int i, int j){
    return adj[i][j];
}

void Graphe::setEdgeValue(int i, int j, float f){
    if(i>=0 && i<n && j >=0 && j<n)
        adj[i][j] = f;
    findMaxadj();
}



void Graphe::drawArrow(QPainter * qp, int i, int j, float ww, float hh){

    int x1 =  (float) noeuds[i].getx()*ww;
    int y1 =  (float) noeuds[i].gety()*hh;
    int x2 = (float) noeuds[j].getx()*ww;
    int y2 = (float) noeuds[j].gety()*hh;

    float alpha = (float) atan2(y2 - y1, x2 - x1);
    float dalpha = 10./180*3.14159;

    float alpha1 = noeuds[i].askAngle(alpha+dalpha);
    float alpha2 = noeuds[j].askAngle((3.14159 + alpha - dalpha));

    float w= qp->window().width()/100.;
    float h= qp->window().height()/100.;
    w = std::min(w, h);
    float beta = 0.55;
    float r = std::min(w*3, (float) 7.);

    float dy = y2 - y1;
    float dx = x2 - x1;
    float dy2, dx2, dxb, dyb, dx2b, dy2b;
    float r0 = sqrt(dx*dx+dy*dy);
    if(r0 < 15)
            r = 0.15*sqrt(dx*dx+dy*dy);

      dx = r*1.6;
      dx2 = dx*cos(alpha2);
      dx *= cos(alpha1);

      float r2 = std::min((float) curve, (float) (curve*r0/200.f));
      dxb = r2*cos(alpha1);
      dyb = r2*sin(alpha1);
      dx2b = r2*cos(alpha2);
      dy2b = r2*sin(alpha2);

      dy = r*1.6 ;
      dy2 = dy*sin(alpha2);
      dy *= sin(alpha1);
    QPainterPath myPath;
    myPath.moveTo(x1+dx, y1+dy);
    myPath.cubicTo(x1+dx+dxb, y1+dy+dyb, x2+dx2+dx2b, y2+dy2+dy2b, x2 + dx2, y2 + dy2);

     qp->drawPath(myPath);

    //qp->drawLine(x1+dx, y1+dy, x2-dx, y2-dy);
    qp->drawLine(x2+0.8*dx2, y2+0.8*dy2, x2+0.8*dx2+(int) (r*cos(alpha2 - beta)), y2 +0.8*dy2 + (int) (r*sin(alpha2 - beta)));
    qp->drawLine(x2+0.8*dx2, y2+0.8*dy2, x2+0.8*dx2+(int) (r*cos(alpha2 + beta)), y2 + 0.8*dy2 + (int) (r*sin(alpha2 + beta)));

      
}

void Graphe::saveGraph(QString & filename){
    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * elgraphe = new TiXmlElement( "Graph" );
        //TiXmlText * text = new TiXmlText( num2str(10) );
        elgraphe->SetAttribute("n", this->n);
        elgraphe->SetAttribute("Title", title.toStdString());
        doc.LinkEndChild( decl );
        doc.LinkEndChild( elgraphe );

     TiXmlElement * elnodes = new TiXmlElement("Nodes");
     TiXmlElement * elnode;
        for(int i=0; i<n; i++){
            elnode = new TiXmlElement("Node");
            elnode->SetAttribute("id", i);
            if(!labels[i].isEmpty())
                elnode->SetAttribute("Label", labels[i].toStdString());
            elnode->SetAttribute("x", this->noeuds[i].getx());
            elnode->SetAttribute("y", this->noeuds[i].gety());
            elnodes->LinkEndChild(elnode);
        }


        elgraphe->LinkEndChild( elnodes );

        TiXmlElement * elarrows = new TiXmlElement("Arrows");
        TiXmlElement * elarrow;
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                if(this->adj[i][j]>threshold){
                    elarrow = new TiXmlElement("Arrow");
                    elarrow->SetAttribute("From", i);
                    elarrow->SetAttribute("To", j);
                    elarrow->SetAttribute("Value", GrapheXml::num2str<float>(adj[i][j], 7));
                    elarrows->LinkEndChild(elarrow);
                }
            }
        }
        elgraphe->LinkEndChild(elarrows);

        TiXmlElement * elconfig = new TiXmlElement("Config");
        elconfig->SetAttribute("ScaleMax", GrapheXml::num2str<float>(maxadj, 7));
        elgraphe->LinkEndChild(elconfig);

        doc.SaveFile(filename.toStdString());
//Nodes
        //Coeffs (adj)

}

void Graphe::saveA(QString &folder){
    std::ofstream file;
    if(matA != NULL){
        for(int k=0; k<p; k++){
            file.open((folder.toStdString()+QString::number(k+1).toStdString()+".txt").c_str());
            for(int i=0; i<n; i++){
                for(int j=0; j<n; j++)
                   file << QString::number(matA[i][j][k]).toStdString() << " ";
                file << std::endl;
                qDebug() << k;
            }
            file.close();
        }

    }
}

void Graphe::addArrow(int i, int j, float value){
    if(i>=0 && i<n){
        if(j>=0 && j<n){
            adj[i][j] = value;
            if(value > maxadj)
                maxadj = value;
        }
    }

}

QString Graphe::label(int i){
    if(labels == NULL)
        return QString::number(i);
    if(labels[i].isEmpty())
        return QString::number(i);
    return labels[i];
}

void Graphe::importLabels(QString &filename){
    delete[] labels;
    labels = new QString[n];

    //Init
    for(int i=0; i<n; i++)
        labels[i]=QString();

    int i =0;

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        //QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while (!in.atEnd() && i<n )
      {
        labels[i] = in.readLine();
        ;
        i++;
      }

    file.close();

}


void Graphe::importTemplate(QString & filename){

    TiXmlDocument doc(filename.toStdString());
    if(doc.LoadFile()){
        //TiXmlHandle hDoc(&doc);
        TiXmlElement * pRoot;
        TiXmlElement * pChildren;
        std::string temps;
        TiXmlElement * pGraph;
        pGraph = doc.FirstChildElement("Graph");
        if(pGraph){
            pRoot = pGraph->FirstChildElement("Nodes");
            int i, x, y;
            if(pRoot){
                pChildren = pRoot->FirstChildElement("Node");
                while(pChildren){
                    pChildren->QueryIntAttribute("id", &i);
                    if(i<n){
                        pChildren->QueryIntAttribute("x", &x);
                        pChildren->QueryIntAttribute("y", &y);
                        noeuds[i]= Noeud(x,y);
                        //qDebug()<<i;
                        if(pChildren->QueryStringAttribute("Label", &temps) == TIXML_SUCCESS){
                            //qDebug()<<QString(temps.c_str());
                            labels[i]= QString(temps.c_str());

                        }
                    }
                    pChildren = pChildren->NextSiblingElement("Node");
                }
            }
        }
    }
}

void Graphe::setOptions(graphOptions &options){
    type = options.style;
    drawLabel = options.displayLabels;
    displayTitle = options.displayTitle;
}

Graphe::~Graphe(){
    for(int i=0; i<n; i++)
        noeuds[i].deleteEmpty();
    delete[] noeuds;
        for(int i =0; i<this->n; i++){
           delete[] adj[i];
        }
        delete[] adj;
    if(matA != NULL){
      for(int i=0; i<n; i++){
          for(int j=0; j<n; j++){
              delete[] matA[i][j];
          }
          delete[] matA[i];
      }
      delete[] matA;
    }
     delete[] labels;
}

