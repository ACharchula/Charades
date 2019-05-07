//
// Created by adam on 01.05.19.
//

#ifndef CPP_CLIENT_DRAWSCENE_H
#define CPP_CLIENT_DRAWSCENE_H


#include <QtWidgets/QGraphicsScene>

class DrawScene : public QGraphicsScene {
    Q_OBJECT

public:
    DrawScene();
    void temp(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent (QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent (QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent (QGraphicsSceneMouseEvent *event) override;
    void updateScene();
};


#endif //CPP_CLIENT_DRAWSCENE_H