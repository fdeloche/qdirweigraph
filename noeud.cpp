#include <noeud.h>

Noeud::Noeud(){}

Noeud::Noeud(int x, int y):x(x), y(y){
}

void Noeud::setCoord(int x, int y){
    this->x=x;
    this->y=y;
}

int Noeud::getx(){
    return this->x;
}

int Noeud::gety(){
    return this->y;
}
