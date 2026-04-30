#include <QtTest>
#include "ScoreManager.h"

class TestScoreManager : public QObject
{
    Q_OBJECT

private slots:
    void init() {
        ScoreManager sm;
        sm.clearLeaderboard();
    }

    void testInit() {
        ScoreManager sm;
        QCOMPARE(sm.getScore(), 0);
    }

    void testAddScore() {
        ScoreManager sm;
        sm.addScore(10);
        QCOMPARE(sm.getScore(), 10);
        
        sm.addScore(25);
        QCOMPARE(sm.getScore(), 35);
        
        // Test ignore negative points
        sm.addScore(-10);
        QCOMPARE(sm.getScore(), 35);
    }

    void testReset() {
        ScoreManager sm;
        sm.addScore(50);
        QCOMPARE(sm.getScore(), 50);
        
        sm.reset();
        QCOMPARE(sm.getScore(), 0);
    }

    void testLeaderboard() {
        ScoreManager sm;
        
        // Add scores
        sm.addScore(100);
        sm.saveScore(30.0f);
        
        sm.reset();
        sm.addScore(200);
        sm.saveScore(25.0f);
        
        sm.reset();
        sm.addScore(200);
        sm.saveScore(20.0f); // Faster, should be #1
        
        sm.reset();
        sm.addScore(50);
        sm.saveScore(40.0f);
        
        QList<QPair<int, float>> tops = sm.getTopScores();
        QCOMPARE(tops.size(), 4);
        
        // Check sorting
        QCOMPARE(tops[0].first, 200);
        QCOMPARE(tops[0].second, 20.0f); // Highest score, fastest
        
        QCOMPARE(tops[1].first, 200);
        QCOMPARE(tops[1].second, 25.0f);
        
        QCOMPARE(tops[2].first, 100);
        QCOMPARE(tops[3].first, 50);
        
        // Test limit to 5
        sm.reset(); sm.addScore(300); sm.saveScore(10.0f);
        sm.reset(); sm.addScore(400); sm.saveScore(10.0f);
        
        tops = sm.getTopScores();
        QCOMPARE(tops.size(), 5);
        QCOMPARE(tops[0].first, 400);
        // The 50 score should be pushed out
        QCOMPARE(tops.last().first, 100);
    }
};

QTEST_MAIN(TestScoreManager)
#include "tst_scoremanager.moc"
