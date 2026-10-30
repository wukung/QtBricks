#include <QtTest>
#include "GameWidget.h"

// Widget tests need a platform plugin; run with QT_QPA_PLATFORM=offscreen
// when no display is available.
class TestGameWidget : public QObject
{
    Q_OBJECT

private slots:
    void testInitialLayout() {
        GameWidget w;
        QCOMPARE(w.remainingBricks(), 5 * 10);
        QVERIFY(!w.isGameStarted());
        QVERIFY(!w.isGameOver());
        QVERIFY(!w.isGameWon());
        QVERIFY(w.ballRect().isValid());
        QVERIFY(w.paddleRect().isValid());
    }

    void testSpaceStartsGame() {
        GameWidget w;
        w.show();
        QVERIFY(QTest::qWaitForWindowExposed(&w));
        QTest::keyClick(&w, Qt::Key_Space);
        QVERIFY(w.isGameStarted());
    }

    void testPaddleMovesRightAfterStart() {
        GameWidget w;
        w.show();
        QVERIFY(QTest::qWaitForWindowExposed(&w));
        const qreal x0 = w.paddleRect().x();
        QTest::keyClick(&w, Qt::Key_Space);
        QTest::keyPress(&w, Qt::Key_Right);
        QTest::qWait(300);
        QTest::keyRelease(&w, Qt::Key_Right);
        // 450 px/s * ~0.3 s ≈ +135 px; inequality tolerates timing jitter.
        QVERIFY(w.paddleRect().x() > x0);
    }
};

QTEST_MAIN(TestGameWidget)
#include "tst_gamewidget.moc"
