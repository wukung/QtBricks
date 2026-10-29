#include <QtTest>
#include "Collision.h"

class TestCollision : public QObject
{
    Q_OBJECT

private slots:
    void testSideTop() {
        // Ball overlapping the top face of the paddle.
        QRectF ball(95, 90, 16, 16);
        QRectF paddle(50, 100, 100, 15);
        QCOMPARE(collisionSide(ball, paddle), CollisionSide::Top);
    }

    void testSideBottom() {
        QRectF ball(95, 110, 16, 16);
        QRectF paddle(50, 100, 100, 15);
        QCOMPARE(collisionSide(ball, paddle), CollisionSide::Bottom);
    }

    void testSideLeft() {
        QRectF ball(40, 102, 16, 16);
        QRectF paddle(50, 100, 100, 15);
        QCOMPARE(collisionSide(ball, paddle), CollisionSide::Left);
    }

    void testSideRight() {
        QRectF ball(144, 102, 16, 16);
        QRectF paddle(50, 100, 100, 15);
        QCOMPARE(collisionSide(ball, paddle), CollisionSide::Right);
    }

    void testTieBreakIsDeterministic() {
        // Equal top/left penetration must always resolve to Top,
        // never depend on float == tie-breaking.
        QRectF moving(0, 0, 10, 10);
        QRectF solid(5, 5, 10, 10);
        QCOMPARE(collisionSide(moving, solid), CollisionSide::Top);
    }

    void testPaddleBounceCentered() {
        QRectF paddle(50, 100, 100, 15);
        QCOMPARE(paddleBounceSpeedX(100.0, paddle, 8.0, 600.0), 0.0);
    }

    void testPaddleBounceClamped() {
        QRectF paddle(50, 100, 100, 15);
        // Way off-center: raw value would exceed maxAbs.
        QCOMPARE(paddleBounceSpeedX(1000.0, paddle, 8.0, 600.0), 600.0);
        QCOMPARE(paddleBounceSpeedX(-1000.0, paddle, 8.0, 600.0), -600.0);
        // In-range offset passes through: (120-100)*8 = 160.
        QCOMPARE(paddleBounceSpeedX(120.0, paddle, 8.0, 600.0), 160.0);
    }
};

QTEST_MAIN(TestCollision)
#include "tst_collision.moc"
