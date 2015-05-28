#include "drawwidget.h"
#include "graphe.h"
#include <QDebug>
#include "coord.h"
#include <QMessageBox>
#include <QInputDialog>

DrawWidget::DrawWidget(){
    gr=NULL;
    selectednode=NULL;
}

DrawWidget::DrawWidget(Graphe * gr): gr(gr){
     selectednode=NULL;
}

void DrawWidget::setGraph(Graphe * graph){
  gr=graph;
}

void DrawWidget::paintEvent(QPaintEvent * event){
    QPainter painter(this);
    if(gr){
        painter.setRenderHint(QPainter::Antialiasing);
        this->gr->draw(&painter);
    }
}

void DrawWidget::mousePressEvent(QMouseEvent * event){
    if(event->button() == Qt::LeftButton){

    int w = this->size().width();
    int h = this->size().height();
    int x = (int) event->pos().x()*100./w;
    int y = (int) event->pos().y()*100./h;
    if(gr){
        selectednode = gr->clickNode(x, y, 3);
        //qDebug() << selectednode;
        if(selectednode)
            this->setCursor(Qt::ClosedHandCursor);
    }
    }
    }

void DrawWidget::mouseDoubleClickEvent( QMouseEvent * e )
{
    if ( e->button() == Qt::LeftButton )
    {
        int w = this->size().width();
        int h = this->size().height();
        int x = (int) e->pos().x()*100./w;
        int y = (int) e->pos().y()*100./h;
        if(gr){
            Coord c = gr->clickEdge(x, y, 3);
            if(c.x != -1){
              /*  QMessageBox mess;
                 mess.setText("Edge " + QString::number(c.x) + " -> " + QString::number(c.y));
                mess.exec();*/
                bool ok;
                double d = QInputDialog::getDouble(this, "Set value", "Value for edge " + QString::number(c.x) + " -> " + QString::number(c.y) + " : ", gr->valueEdge(c.x, c.y), -10, 10, 2, &ok);
                gr->setEdgeValue(c.x, c.y, (float) d);
                this->update();
            }
        }
    }
}

void DrawWidget::mouseReleaseEvent(QMouseEvent * event){
    int w = this->size().width();
    int h = this->size().height();
    int x = (int) event->pos().x()*100./w;
    int y = (int) event->pos().y()*100./h;
    if(selectednode){
        selectednode->setCoord(x, y);
        this->update();
    }
    selectednode = NULL;
    this->setCursor(Qt::ArrowCursor);
}

/*
void DrawWidget::changeCurve(int value){
    if(gr){
        gr->setCurve(value);
        this->update();
    }
}
*/
