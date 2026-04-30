#ifndef BRICK_H
#define BRICK_H

#include <QRectF>
#include <QPainter>
#include <QColor>

class Brick {
public:
    Brick();

    void init(qreal startX, qreal startY, qreal width, qreal height, const QColor &color);
    void draw(QPainter *painter) const;

    QRectF getRect() const;
    bool isDestroyed() const;
    void setDestroyed(bool d);

private:
    QRectF rect;
    QColor brickColor;
    bool destroyed;
};

#endif // BRICK_H
