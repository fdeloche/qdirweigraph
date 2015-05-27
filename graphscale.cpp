#include "graphscale.h"

#include <QPen>

GraphScale::GraphScale()
{

}

void GraphScale::paintEvent(QPaintEvent * event){
    QPainter painter(this);
    this->paint(&painter);
}

void GraphScale::paint(QPainter *qp){
    int w = qp->window().width();
    int h = qp->window().height()-40;
    int y = 20;

    float ratio, alpha, thick;
    int nrep = h/50;
    if(h > 150 & maxvalue > 0){
        for(int i=100; i>0; i=i-100/nrep){
        //100% 80% ...
        ratio = i/100.;
        int color_r, color_v, color_b;
        color_r = 0;
        color_v = 0;
        color_b = 0;


        if(type==COLORGRAPH){
         //if adj[i][j]<0.5 maxadj, change alpha, >0.6 ou >0.5change color < 1 change color and thickness
        alpha = (ratio<0.8) ? 255*(ratio/(0.8)) : 255;
        if(colorscale==GREENRED){
        color_r = (ratio<0.5) ? 0 : 255.*(ratio-0.3)/0.7;
        color_v = (ratio<0.5) ? 175 : 175 - 175*(ratio-0.3)/0.7;
        color_b = 0;
        }
        if(colorscale==BLUEPURPLE){
            color_r = (ratio<0.6) ? 40 : 40 + 215.*(ratio-0.5)/0.5;
            color_v=100;
            //color_v = (ratio<0.3) ? 175 : 175 - 175*(ratio-0.3)/0.7;
            color_b = (ratio<0.6) ? 200 : 200 + 55.*(ratio-0.5)/0.5;
        }
        thick = (ratio<0.2) ? 0.5 : ratio*2.5;
        thick = (ratio<0.6) ? thick : ratio*6. - 2.1f;
        qp->setPen(QPen(QColor(color_r, color_v, color_b, alpha), thick, Qt::SolidLine));
        qp->setBrush(Qt::NoBrush);
        }
        if(type==BLACKGRAPH){
            //if adj[i][j]<0.3 maxadj, change alpha, else change thickness (grosso modo)
        alpha = (ratio<0.3) ? 255*((0.3*ratio)) : 255;
        thick = (ratio<0.2) ? 0.5 : ratio*5.;
        qp->setPen(QPen(QColor(color_r, color_v, color_b, alpha), thick, Qt::SolidLine));
        qp->setBrush(Qt::NoBrush);
        }

        qp->drawLine(15, y, 45, y);
        y+=h/(2*nrep);
        qp->drawText(20, y, QString::number(ratio*maxvalue, 'g', 3));
        y += h/(2*nrep);
    }
    }
}
