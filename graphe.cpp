#include <graphe.h>

#include <QString>
#include <QDebug>
#include <cmath>

#include "tinyxml.h"
#include "graphexml.h"

#include <stdlib.h>

#include <QPainterPath>

#include <locale>
Graphe::Graphe(){
    n=0;
    maxadj=0;
    title="";
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

            adj = new float*[n];

            for(int i=0; i<n; i++){
                adj[i] = new float [n];
                for(int j=0; j<n; j++){
                    adj[i][j]=0.;
                }
            }
            TiXmlElement * pRoot;
            TiXmlElement * pChildren;
            pRoot = pGraph->FirstChildElement("Nodes");
            if(pRoot){
                pChildren = pRoot->FirstChildElement("Node");
                while(pChildren){
                    pChildren->QueryIntAttribute("id", &i);

                    pChildren->QueryIntAttribute("x", &x);
                    pChildren->QueryIntAttribute("y", &y);
                    noeuds[i]= Noeud(x,y);
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
    //qDebug() << w << qPrintable(" ") << h;
    qp->setBrush(Qt::black);
    int x, y;

    int r = 2;

    //NODES
    for(int i =0; i<this->n; i++){
        noeuds[i].reset();
        x = noeuds[i].getx()*w;
        y= noeuds[i].gety()*h;
        //qDebug() << i << noeuds[i].getx() <<  noeuds[i].gety();
        qp->drawEllipse(x-r/2, y-r/2, r, r);
        if(x<w*50.){
            if(y<h*50.){
                qp->drawText(x-13, y-5, QString::number(i));
            }else{
                qp->drawText(x-13, y+13, QString::number(i));
            }
        }else{
            if(y<h*50.){
                qp->drawText(x+5, y-5, QString::number(i));
             }else{
                qp->drawText(x+5, y+13, QString::number(i));
            }
        }
    }
    float alpha;
    float thick;
    float ratio;
    for(int i =0; i<this->n; i++){
        for(int j =0; j<this->n; j++){
            if(adj[i][j] != 0.){
                if(maxadj!=0){

                    if(type==COLORGRAPH){
                     //if adj[i][j]<0.5 maxadj, change alpha, >0.6 ou >0.5change color < 1 change color and thickness
                    ratio = adj[i][j]/maxadj;
                    alpha = (ratio<0.8) ? 255*(adj[i][j]/(0.8*maxadj)) : 255;
                    if(colorscale==GREENRED){
                    color_r = (ratio<0.5) ? 0 : 255.*(ratio-0.3)/0.7;
                    color_v = (ratio<0.5) ? 175 : 175 - 175*(ratio-0.3)/0.7;
                    color_b = 0;
                    }
                    if(colorscale==BLUEPURPLE){
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

Graphe::~Graphe(){
    for(int i=0; i<n; i++)
        noeuds[i].deleteEmpty();
    delete[] noeuds;
        for(int i =0; i<this->n; i++){
           delete[] adj[i];
        }
        delete[] adj;
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
    float beta = 0.4;
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
    qp->drawLine(x2+dx2, y2+dy2, x2+dx2+(int) (r*cos(alpha2 - beta)), y2 +dy2 + (int) (r*sin(alpha2 - beta)));
    qp->drawLine(x2+dx2, y2+dy2, x2+dx2+(int) (r*cos(alpha2 + beta)), y2 + dy2 + (int) (r*sin(alpha2 + beta)));

      
}

void Graphe::saveGraph(QString & filename){
    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * elgraphe = new TiXmlElement( "Graph" );
        //TiXmlText * text = new TiXmlText( num2str(10) );
        elgraphe->SetAttribute("n", this->n);
        doc.LinkEndChild( decl );
        doc.LinkEndChild( elgraphe );

     TiXmlElement * elnodes = new TiXmlElement("Nodes");
     TiXmlElement * elnode;
        for(int i=0; i<n; i++){
            elnode = new TiXmlElement("Node");
            elnode->SetAttribute("id", i);
            elnode->SetAttribute("x", this->noeuds[i].getx());
            elnode->SetAttribute("y", this->noeuds[i].gety());
            elnodes->LinkEndChild(elnode);
        }


        elgraphe->LinkEndChild( elnodes );

        TiXmlElement * elarrows = new TiXmlElement("Arrows");
        TiXmlElement * elarrow;
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                if(this->adj[i][j]!=0){
                    elarrow = new TiXmlElement("Arrow");
                    elarrow->SetAttribute("From", i);
                    elarrow->SetAttribute("To", j);
                    elarrow->SetAttribute("Value", GrapheXml::num2str<float>(adj[i][j], 7));
                    elarrows->LinkEndChild(elarrow);
                }
            }
        }
        elgraphe->LinkEndChild(elarrows);
        doc.SaveFile(filename.toStdString());
//Nodes
        //Coeffs (adj)

}

