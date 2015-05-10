#ifndef GRAPHE_H
#define GRAPHE_H

#include <QPainter>
#include <QTextStream>
#include "noeud.h"
#include "coord.h"

class Graphe{
private:
    Noeud * noeuds;
    float * * adj;
    int n;

public:
    Graphe();
    Graphe(Noeud * noeuds, float * * adj, int n);
    void draw(QPainter * qp);
    int getn();
    void printNodes(QTextStream& stream);
    void printCoeffs(QTextStream& stream);
    ~Graphe();
    Noeud * clickNode(int x, int y, int r);
    Coord clickEdge(int x, int y, int r);
    float valueEdge(int i, int j);
    void setEdgeValue(int i, int j, float f);
    void drawArrow(QPainter * qp, int x1, int y1, int x2, int y2);
};

#endif // GRAPHE_H
