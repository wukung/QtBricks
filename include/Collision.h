#ifndef COLLISION_H
#define COLLISION_H

#include <QRectF>

enum class CollisionSide { Top, Bottom, Left, Right };

// Minimum-penetration side between a moving rect and a solid rect.
// Precondition: the rects intersect. Ties resolve deterministically in
// Top, Bottom, Left, Right order (no float == comparison).
// All math in qreal to avoid float/qreal precision mixing.
CollisionSide collisionSide(const QRectF &moving, const QRectF &solid);

// Paddle bounce: horizontal speed from the hit offset relative to the
// paddle center, clamped to [-maxAbs, maxAbs] so long rallies cannot
// accelerate the ball without bound.
qreal paddleBounceSpeedX(qreal ballCenterX, const QRectF &paddleRect,
                         qreal factor, qreal maxAbs);

#endif // COLLISION_H
