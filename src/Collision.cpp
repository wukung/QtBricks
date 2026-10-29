#include "Collision.h"
#include <QtMath>

CollisionSide collisionSide(const QRectF &moving, const QRectF &solid)
{
    const qreal overlapTop = moving.bottom() - solid.top();
    const qreal overlapBottom = solid.bottom() - moving.top();
    const qreal overlapLeft = moving.right() - solid.left();
    const qreal overlapRight = solid.right() - moving.left();

    if (overlapTop <= overlapBottom && overlapTop <= overlapLeft
        && overlapTop <= overlapRight) {
        return CollisionSide::Top;
    }
    if (overlapBottom <= overlapLeft && overlapBottom <= overlapRight) {
        return CollisionSide::Bottom;
    }
    if (overlapLeft <= overlapRight) {
        return CollisionSide::Left;
    }
    return CollisionSide::Right;
}

qreal paddleBounceSpeedX(qreal ballCenterX, const QRectF &paddleRect,
                         qreal factor, qreal maxAbs)
{
    const qreal paddleCenter = paddleRect.left() + paddleRect.width() / 2.0;
    return qBound(-maxAbs, (ballCenterX - paddleCenter) * factor, maxAbs);
}
