#include "Paddle.h"

Paddle::Paddle() : moveSpeed(0), moveLeft(false), moveRight(false) {}

void Paddle::init(qreal startX, qreal startY, qreal width, qreal height, qreal speed) {
    rect.setRect(startX, startY, width, height);
    moveSpeed = speed;
    moveLeft = false;
    moveRight = false;
}

void Paddle::update(float step, float windowWidth) {
    if (moveLeft) {
        rect.translate(-moveSpeed * step, 0);
        if (rect.left() < 0) rect.moveLeft(0);
    }
    if (moveRight) {
        rect.translate(moveSpeed * step, 0);
        if (rect.right() > windowWidth) rect.moveRight(windowWidth);
    }
}

void Paddle::draw(QPainter *painter) const {
    painter->setBrush(QColor(100, 200, 255));
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(rect, 5, 5);
}

QRectF Paddle::getRect() const { return rect; }
void Paddle::setMovingLeft(bool moving) { moveLeft = moving; }
void Paddle::setMovingRight(bool moving) { moveRight = moving; }
