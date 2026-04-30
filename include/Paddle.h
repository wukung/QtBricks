#ifndef PADDLE_H
#define PADDLE_H

#include <QRectF>
#include <QPainter>

class Paddle {
public:
    Paddle();

    void init(qreal startX, qreal startY, qreal width, qreal height, qreal speed);
    void update(float step, float windowWidth);
    void draw(QPainter *painter) const;

    QRectF getRect() const;
    void setMovingLeft(bool moving);
    void setMovingRight(bool moving);

private:
    QRectF rect;
    qreal moveSpeed;
    bool moveLeft;
    bool moveRight;
};

#endif // PADDLE_H
