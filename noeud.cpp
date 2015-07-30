#include <noeud.h>

#include <QDebug>

Noeud::Noeud(){ maxarcs=0;}

Noeud::Noeud(int x, int y):x(x), y(y){
    maxarcs = 10;
    empty = new bool[30];
    this->reset();
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

void Noeud::reset(){

    for(int i=0; i<maxarcs; i++)
        empty[i]=true;
}

float Noeud::askAngle(float a){
    int i = ((a/(2*3.1416)*maxarcs)+0.5f);
    i = (i+maxarcs)%maxarcs;
    if(empty[i]){
        empty[i] = false;
        return i*2*3.1416/maxarcs;
    }else{
        int left, right;
           for(int step=1; step<maxarcs/2; step++){
            left = (i-step+maxarcs)%maxarcs;
            right = (i+step+maxarcs)%maxarcs;
            if(empty[left]){
                empty[left] = false;
                return (left)*2*3.1416/maxarcs;
            }
            if(empty[right]){
                empty[right] = false;
                return (right)*2*3.1416/maxarcs;
            }
            step += 1;
        }
    }
    //if we have failed to return something, we return the initial value
return a;
}


void Noeud::deleteEmpty(){
    delete[] empty;
}

Noeud::~Noeud(){

}
