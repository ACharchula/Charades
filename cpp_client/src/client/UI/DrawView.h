//
// Created by adam on 01.05.19.
//

#ifndef CPP_CLIENT_DRAWVIEW_H
#define CPP_CLIENT_DRAWVIEW_H


#include <QtWidgets/QGraphicsView>

class DrawView : public QGraphicsView {
Q_OBJECT

private:
    const size_t height;
    const size_t width;

public:
    explicit DrawView(size_t height, size_t width);
};


#endif //CPP_CLIENT_DRAWVIEW_H
