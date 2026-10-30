#include <QtTest>
#include <QTemporaryDir>
#include <limits>
#include "ScoreManager.h"

class TestScoreManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {
        // Isolate tests from the user's real leaderboard: redirect the
        // backing store to a temp ini file. Works on every OS because the
        // override bypasses NativeFormat (registry / CFPreferences);
        // production never sets it, so no score migration happens.
        QVERIFY(settingsDir.isValid());
        ScoreManager::setSettingsFileOverride(
            settingsDir.filePath("TestLeaderboard.ini"));
    }

    void cleanupTestCase() {
        ScoreManager::clearSettingsFileOverride();
    }

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

    void testTamperedEntriesFiltered() {
        QSettings raw(ScoreManager::settingsFileOverride(),
                      QSettings::IniFormat);
        raw.beginWriteArray("Leaderboard");
        raw.setArrayIndex(0);
        raw.setValue("score", 100);
        raw.setValue("duration", 10.0);
        raw.setArrayIndex(1);
        raw.setValue("score", -50); // tampered: negative score
        raw.setValue("duration", 5.0);
        raw.setArrayIndex(2);
        raw.setValue("score", 80); // tampered: NaN duration
        raw.setValue("duration", std::numeric_limits<double>::quiet_NaN());
        raw.setArrayIndex(3);
        raw.setValue("score", 70); // tampered: unparseable duration
        raw.setValue("duration", "abc");
        raw.endArray();
        raw.sync();

        ScoreManager sm;
        QList<QPair<int, float>> tops = sm.getTopScores();
        QCOMPARE(tops.size(), 1);
        QCOMPARE(tops[0].first, 100);
    }

private:
    QTemporaryDir settingsDir;
};

QTEST_MAIN(TestScoreManager)
#include "tst_scoremanager.moc"
