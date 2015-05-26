#ifndef GRAPHSCALE_H
#define GRAPHSCALE_H

#include <QWidget>
#include <QPainter>

class GraphScale : public QWidget{
private:
    float maxvalue = 0;
    float maxthick = 5;

public:
    GraphScale();

    void setMaxvalue(float value){
        maxvalue = value;
    }

    float getMaxvalue(){
        return maxvalue;
    }

    protected:
    void paintEvent(QPaintEvent * event);
};


#endif // GRAPHSCALE_H
