#include <QtTest>
#include "Paddle.h"

class TestPaddle : public QObject
{
    Q_OBJECT

private slots:
    void testInit() {
        Paddle paddle;
        paddle.init(50.0, 550.0, 100.0, 15.0, 300.0);
        
        QRectF rect = paddle.getRect();
        QCOMPARE(rect.x(), 50.0);
        QCOMPARE(rect.y(), 550.0);
        QCOMPARE(rect.width(), 100.0);
        QCOMPARE(rect.height(), 15.0);
    }

    void testMoveLeft() {
        Paddle paddle;
        paddle.init(100.0, 500.0, 100.0, 15.0, 100.0);
        
        paddle.setMovingLeft(true);
        paddle.update(0.5f, 800.0); // dt=0.5, speed=100 -> dx=-50
        
        QCOMPARE(paddle.getRect().x(), 50.0);
    }

    void testMoveRight() {
        Paddle paddle;
        paddle.init(100.0, 500.0, 100.0, 15.0, 100.0);
        
        paddle.setMovingRight(true);
        paddle.update(1.0f, 800.0); // dt=1.0, speed=100 -> dx=100
        
        QCOMPARE(paddle.getRect().x(), 200.0);
    }

    void testBoundaryConstraints() {
        Paddle paddle;
        // Start near left edge
        paddle.init(10.0, 500.0, 100.0, 15.0, 100.0);
        
        paddle.setMovingLeft(true);
        paddle.update(1.0f, 800.0); // Try to move -100
        
        // It should stop exactly at 0
        QCOMPARE(paddle.getRect().x(), 0.0);
    }
};

QTEST_MAIN(TestPaddle)
#include "tst_paddle.moc"
