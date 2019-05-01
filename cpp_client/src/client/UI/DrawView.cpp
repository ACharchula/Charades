//
// Created by adam on 01.05.19.
//

#include "DrawView.h"

DrawView::DrawView(size_t height, size_t width) : height(height), width(width) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setMinimumSize(300,300);
    setMaximumSize(300,300);
}