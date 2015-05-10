#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include <QPainter>
#include <graphe.h>
#include <QMouseEvent>
#include <noeud.h>

class DrawWidget : public QWidget{
private:
    Graphe * gr;
    Noeud * selectednode;

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

