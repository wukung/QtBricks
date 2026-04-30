#ifndef BALL_H
#define BALL_H

#include <QRectF>
#include <QPainter>

class Ball {
public:
    Ball();

    void init(qreal startX, qreal startY, qreal radius, qreal speedX, qreal speedY);
    void move(float step);
    void draw(QPainter *painter) const;

    QRectF getRect() const;
    void setRect(const QRectF &rect);
    
    qreal getSpeedX() const;
    qreal getSpeedY() const;
    void setSpeedX(qreal vx);
    void setSpeedY(qreal vy);

    void moveLeft(qreal pos);
    void moveRight(qreal pos);
    void moveTop(qreal pos);
    void moveBottom(qreal pos);

private:
    QRectF rect;
    qreal speedX;
    qreal speedY;
};

#endif // BALL_H
