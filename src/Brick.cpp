#include "Brick.h"

Brick::Brick() : destroyed(false) {}

void Brick::init(qreal startX, qreal startY, qreal width, qreal height, const QColor &color) {
    rect.setRect(startX, startY, width, height);
    brickColor = color;
    destroyed = false;
}

void Brick::draw(QPainter *painter) const {
    if (!destroyed) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(brickColor);
        painter->drawRoundedRect(rect, 3, 3);
    }
}

QRectF Brick::getRect() const { return rect; }
bool Brick::isDestroyed() const { return destroyed; }
void Brick::setDestroyed(bool d) { destroyed = d; }
