#include <QtTest>
#include "Brick.h"

class TestBrick : public QObject
{
    Q_OBJECT

private slots:
    void testInit() {
        Brick brick;
        brick.init(10.0, 20.0, 60.0, 20.0, QColor(255, 0, 0));
        
        QRectF rect = brick.getRect();
        QCOMPARE(rect.x(), 10.0);
        QCOMPARE(rect.y(), 20.0);
        QCOMPARE(rect.width(), 60.0);
        QCOMPARE(rect.height(), 20.0);
        
        QCOMPARE(brick.isDestroyed(), false);
    }

    void testDestroy() {
        Brick brick;
        brick.init(0, 0, 10, 10, QColor(0, 0, 0));
        QCOMPARE(brick.isDestroyed(), false);
        
        brick.setDestroyed(true);
        QCOMPARE(brick.isDestroyed(), true);
    }
};

QTEST_MAIN(TestBrick)
#include "tst_brick.moc"
