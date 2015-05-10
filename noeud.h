#ifndef NOEUD_H
#define NOEUD_H

class Noeud{
private:
    int x;
    int y;

public:
    Noeud();
    Noeud(int x, int y);
    void setCoord(int x, int y);
    int getx();
    int gety();
};

#endif // NOEUD_H

