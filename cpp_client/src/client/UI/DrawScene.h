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
    void mouseReleaseEvent (QGraphicsSceneMouseEvent *event) override;
};


#endif //CPP_CLIENT_DRAWSCENE_H
