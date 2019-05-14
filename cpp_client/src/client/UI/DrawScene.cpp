//
// Created by adam on 01.05.19.
//

#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QDebug>
#include <QtCore/QBuffer>
#include "DrawScene.h"

DrawScene::DrawScene() {
   setSceneRect(0,0,400,400);
}

void DrawScene::mousePressEvent (QGraphicsSceneMouseEvent *event){
    temp(event);
}

void DrawScene::mouseMoveEvent (QGraphicsSceneMouseEvent *event){
   temp(event);
}

void DrawScene::mouseReleaseEvent (QGraphicsSceneMouseEvent *event){
    temp(event);
}

void DrawScene::temp(QGraphicsSceneMouseEvent *event){
   auto mousePosition = event->scenePos();

   int x = static_cast<int>(mousePosition.x());
   int y = static_cast<int>(mousePosition.y());

   double rad = 1;
   addEllipse(x-rad, y-rad, rad*2.0, rad*2.0,
              QPen(), QBrush(Qt::SolidPattern));

   std::cout << x << " " << y << std::endl;
}

void DrawScene::updateScene(QByteArray byteArray) {
    clear();

    QImage image(sceneRect().size().toSize(), QImage::Format_ARGB32);
    image = QImage::fromData(byteArray, "PNG");
    setSceneRect(image.rect());
    addPixmap(QPixmap::fromImage(image));
    update();
}

void DrawScene::saveNextFrame() {
    QImage image(sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    render(&painter);
    image.save("../data/draw/nextFrame.png");
}

QByteArray DrawScene::getScene() {
    QImage image(sceneRect().size().toSize(), QImage::Format_ARGB32);

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    buffer.close();
    return ba;
}
