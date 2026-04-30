#include "Ball.h"
#include <QtMath>

Ball::Ball() : speedX(0), speedY(0) {}

void Ball::init(qreal startX, qreal startY, qreal radius, qreal sx, qreal sy) {
    rect.setRect(startX, startY, radius * 2, radius * 2);
    speedX = sx;
    speedY = sy;
}

void Ball::move(float step) {
    rect.translate(speedX * step, speedY * step);
}

void Ball::draw(QPainter *painter) const {
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 100, 100));
    painter->drawEllipse(rect);
}

QRectF Ball::getRect() const { return rect; }
void Ball::setRect(const QRectF &r) { rect = r; }

qreal Ball::getSpeedX() const { return speedX; }
qreal Ball::getSpeedY() const { return speedY; }
void Ball::setSpeedX(qreal vx) { speedX = vx; }
void Ball::setSpeedY(qreal vy) { speedY = vy; }

void Ball::moveLeft(qreal pos) { rect.moveLeft(pos); }
void Ball::moveRight(qreal pos) { rect.moveRight(pos); }
void Ball::moveTop(qreal pos) { rect.moveTop(pos); }
void Ball::moveBottom(qreal pos) { rect.moveBottom(pos); }
