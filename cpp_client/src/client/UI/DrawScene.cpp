//
// Created by adam on 01.05.19.
//

#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QDebug>
#include "DrawScene.h"

DrawScene::DrawScene() {
   setSceneRect(0,0,400,400);

}

void DrawScene::mousePressEvent (QGraphicsSceneMouseEvent *event){
    std::cout<<"@";
    temp(event);
}

void DrawScene::mouseMoveEvent (QGraphicsSceneMouseEvent *event){
   std::cout<<"$";
   temp(event);
}

void DrawScene::mouseReleaseEvent (QGraphicsSceneMouseEvent *event){
    std::cout<<"%";
    temp(event);
}

void DrawScene::temp(QGraphicsSceneMouseEvent *event){
   auto mousePosition = event->scenePos();

   int x = static_cast<int>(mousePosition.x());
   int y = static_cast<int>(mousePosition.y());

   double rad = 1;
   addEllipse(x-rad, y-rad, rad*2.0, rad*2.0,
              QPen(), QBrush(Qt::SolidPattern));

   if(x>300){
       QImage image(sceneRect().size().toSize(), QImage::Format_ARGB32);  // Create the image with the exact size of the shrunk scene
       image.fill(Qt::transparent);                                              // Start all pixels transparent

       QPainter painter(&image);
       render(&painter);
       image.save("file_name2.png");
   }
   if(x<100){
       clear();
       QImage image(sceneRect().size().toSize(), QImage::Format_ARGB32);  // Create the image with the exact size of the shrunk scene
       image.load("file_name.png");
       setSceneRect(image.rect());
       addPixmap(QPixmap::fromImage(image));
       update();
   }
   std::cout << x << " " << y << std::endl;
}

void DrawScene::method(const QString& value) {
    qDebug() << value;
}
