#ifndef NOEUD_H
#define NOEUD_H

class Noeud{
private:
    bool * empty;
    int maxarcs;
    int x;
    int y;

public:
    Noeud();
    Noeud(int x, int y);
    void setCoord(int x, int y);
    int getx();
    int gety();
    void reset();
    float askAngle(float a);
    void deleteEmpty();
    void setMaxArcs(int max){
        maxarcs = max;
    }

    ~Noeud();
};

#endif // NOEUD_H

