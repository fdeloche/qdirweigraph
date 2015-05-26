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

    if(h > 150 & maxvalue > 0){
        for(int i=100; i>0; i=i-20){
        //100% 80% ...
        ratio = i/100.;
        alpha = (ratio<0.3) ? 255*ratio/0.3 : 255;
        thick = (ratio<0.2) ? 0.5 : ratio*maxthick;
        qp->setPen(QPen(QColor(0, 0, 0, alpha), thick, Qt::SolidLine));
        qp->drawLine(15, y, 45, y);
        y+=h/12;
        qp->drawText(20, y, QString::number(ratio*maxvalue, 'g', 3));
        y += h/12;
        if(i==20)
            i=30;
        }
    }
}
