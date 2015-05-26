#ifndef GRAPHE_H
#define GRAPHE_H

#include <QPainter>
#include <QTextStream>
#include "noeud.h"
#include "coord.h"

#include <QString>


class Graphe{
private:
    Noeud * noeuds;
    float * * adj;
    int n;
    float maxadj;
    void findMaxadj();
    QString title;

public:
    Graphe();
    Graphe(Noeud * noeuds, float * * adj, int n);
    Graphe(QString& filename);
    QString getTitle(){return title;}

    void draw(QPainter * qp);
    int getn();
    void printNodes(QTextStream& stream);
    void printCoeffs(QTextStream& stream);
    ~Graphe();
    Noeud * clickNode(int x, int y, int r);
    Coord clickEdge(int x, int y, int r);
    float valueEdge(int i, int j);
    void setEdgeValue(int i, int j, float f);
    void getEdgeValue(int i, int j){valueEdge(i, j);}
    void drawArrow(QPainter * qp, int x1, int y1, int x2, int y2);

    void saveGraph(QString& filename);
    float getMaxAdj(){return maxadj;}
    float setMaxAdj(float value){maxadj=value;}
    //Graphe * openGraph(std::string &filename);
};

#endif // GRAPHE_H

