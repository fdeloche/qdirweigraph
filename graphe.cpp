#include <graphe.h>

#include <QString>
#include <QDebug>
#include <cmath>

Graphe::Graphe(){
    n=0;
}

Graphe::Graphe(Noeud * noeuds, float * * adj, int n):noeuds(noeuds), adj(adj), n(n){
}

void Graphe::draw(QPainter * qp){
    float w= qp->window().width()/100.;
   float  h= qp->window().height()/100.;
   //qDebug() << w << qPrintable(" ") << h;
   int x, y;
    for(int i =0; i<this->n; i++){
        x = (int) noeuds[i].getx()*w;
        y= (int) noeuds[i].gety()*h;
        qp->drawEllipse(x-1, y-1, 3, 3);
        qp->drawText(x-5, y-5, QString::number(i));
    }
    for(int i =0; i<this->n; i++){
        for(int j =0; j<this->n; j++){
            if(adj[i][j] != 0.){
                qp->setPen(QPen(Qt::black, adj[i][j]*2., Qt::SolidLine));

                qp->drawLine((int) noeuds[i].getx()*w, (int) noeuds[i].gety()*h,(int) noeuds[j].getx()*w, (int) noeuds[j].gety()*h);
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
    int xa, xb, ya, yb, y2;
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
                    y2 = (yb - ya)/(xb- xa)*(x-xa) + ya;
                    //Manhattan distance
                    if( abs(y-y2) <r){
                        return Coord(i, j);
                    }
                }
            }
        }
    }
    return Coord(-1, -1);
}

Graphe::~Graphe(){
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
}
