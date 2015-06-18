#ifndef GRAPHE_H
#define GRAPHE_H

#include <QPainter>
#include <QTextStream>
#include "noeud.h"
#include "coord.h"

#include <QString>

#include "graphoptions.h"

class Graphe{
private:
    Noeud * noeuds;
    float * * adj;
    int n;
    float maxadj;
    float threshold = 0;
    void findMaxadj();
    QString title;
    int color_r = 0;
    int color_b = 0;
    int color_v = 0;
    int type = BLUEPURPLE;
    int curve = 40;
    bool drawLabel = true;
    bool displayTitle = false;
    //Associated matrix
    int p = 1;
    float * * * matA = NULL;
    QString * labels = NULL;
    QString label(int i);

public:
    Graphe();
    Graphe(int n, int p=1);

    Graphe(Noeud * noeuds, float * * adj, int n);
    Graphe(QString& filename);
    QString getTitle(){return title;}
    void setTitle(QString& t){title=t;}

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
    void drawArrow(QPainter * qp, int i, int j, float w,float h);
    void setCurve(int curv){
        curve = curv;
    }
    void setA(int i, int j, int k, float value){
        if(i<n && i>=0 && j>=0 && j<n && k>=0 && k<p)
            matA[i][j][k]=value;
    }

    int getp(){
        return p;
    }

    float getThreshold(){return threshold;}
    void setThreshold(float t){threshold = t;}

    void importTemplate(QString & filename);

    void saveA(QString& folder);

    void saveGraph(QString& filename);
    void importLabels(QString & filename);
    float getMaxAdj(){return maxadj;}
    void setMaxAdj(float value){maxadj=value;}

    void addArrow(int i, int j, float value);
    void setOptions(graphOptions & options);
    //Graphe * openGraph(std::string &filename);
};

#endif // GRAPHE_H

