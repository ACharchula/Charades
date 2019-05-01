//
// Created by adam on 01.05.19.
//

#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include "DrawScene.h"

DrawScene::DrawScene() {

}

void DrawScene::mouseReleaseEvent (QGraphicsSceneMouseEvent *event){
   auto mousePosition = event->scenePos();

   int x = static_cast<int>(mousePosition.x());
   int y = static_cast<int>(mousePosition.y());

   std::cout << x << " " << y << std::endl;
}
