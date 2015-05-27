#ifndef GRAPHSCALE_H
#define GRAPHSCALE_H

#include <QWidget>
#include <QPainter>

#include "graphe.h" //flags

class GraphScale : public QWidget{
private:
    float maxvalue = 0;
    float maxthick = 5;
    int type = COLORGRAPH;
    int colorscale = BLUEPURPLE;

public:
    GraphScale();

    void setMaxvalue(float value){
        maxvalue = value;
    }

    void paint(QPainter * qp);

    float getMaxvalue(){
        return maxvalue;
    }

    protected:
    void paintEvent(QPaintEvent * event);
};


#endif // GRAPHSCALE_H
