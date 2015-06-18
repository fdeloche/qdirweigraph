#ifndef GRAPHSCALE_H
#define GRAPHSCALE_H

#include <QWidget>
#include <QPainter>

#include "graphoptions.h"
//flags

class GraphScale : public QWidget{
private:
    float maxvalue = 0;
    float maxthick = 5;
    int type = BLUEPURPLE;

public:
    GraphScale();

    void setMaxvalue(float value){
        maxvalue = value;
    }

    void paint(QPainter * qp);

    float getMaxvalue(){
        return maxvalue;
    }

    void setOptions(graphOptions &options);


    protected:
    void paintEvent(QPaintEvent * event);
};


#endif // GRAPHSCALE_H
