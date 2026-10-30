#ifndef COLLISION_H
#define COLLISION_H

#include <QRectF>

enum class CollisionSide { Top, Bottom, Left, Right };

// Minimum-penetration side between a moving rect and a solid rect.
// Precondition: the rects intersect. Ties resolve deterministically in
// Top, Bottom, Left, Right order.
CollisionSide collisionSide(const QRectF &moving, const QRectF &solid);

// Paddle bounce: horizontal speed from the hit offset relative to the
// paddle center, clamped to [-maxAbs, maxAbs] as a guard so centers
// outside the reachable range cannot produce unbounded speeds.
qreal paddleBounceSpeedX(qreal ballCenterX, const QRectF &paddleRect,
                         qreal factor, qreal maxAbs);

#endif // COLLISION_H
