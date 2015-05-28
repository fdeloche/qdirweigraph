#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include <QPainter>
#include "graphe.h"
#include <QMouseEvent>
#include "noeud.h"

class DrawWidget : public QWidget{
  //  Q_OBJECT

private:
    Graphe * gr;
    Noeud * selectednode;

//public slots:
    //void changeCurve(int value);

public:
    DrawWidget();
    DrawWidget(Graphe * gr);
    void setGraph(Graphe * gr);

    protected:
    void paintEvent(QPaintEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseDoubleClickEvent( QMouseEvent * e );
};

#endif // DRAWWIDGET_H

