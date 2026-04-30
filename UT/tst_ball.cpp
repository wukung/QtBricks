#include <QtTest>
#include "Ball.h"

class TestBall : public QObject
{
    Q_OBJECT

private slots:
    void testInit() {
        Ball ball;
        ball.init(10.0, 20.0, 5.0, 100.0, -100.0);
        
        QRectF rect = ball.getRect();
        QCOMPARE(rect.x(), 10.0);
        QCOMPARE(rect.y(), 20.0);
        QCOMPARE(rect.width(), 10.0); // radius * 2
        QCOMPARE(rect.height(), 10.0); // radius * 2
        
        QCOMPARE(ball.getSpeedX(), 100.0);
        QCOMPARE(ball.getSpeedY(), -100.0);
    }

    void testMove() {
        Ball ball;
        ball.init(0.0, 0.0, 5.0, 200.0, -100.0);
        
        // Move with dt = 0.5 seconds
        ball.move(0.5f);
        
        QRectF rect = ball.getRect();
        QCOMPARE(rect.x(), 100.0); // 200 * 0.5
        QCOMPARE(rect.y(), -50.0); // -100 * 0.5
    }

    void testSetters() {
        Ball ball;
        ball.init(0, 0, 5, 100, 100);
        
        ball.setSpeedX(-50.0);
        ball.setSpeedY(20.0);
        
        QCOMPARE(ball.getSpeedX(), -50.0);
        QCOMPARE(ball.getSpeedY(), 20.0);
        
        ball.moveRight(500.0);
        QCOMPARE(ball.getRect().right(), 500.0);
    }
};

QTEST_MAIN(TestBall)
#include "tst_ball.moc"
