#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <QSettings>
#include <QList>
#include <QPair>

class ScoreManager {
public:
    ScoreManager();
    
    void reset();
    void addScore(int points);
    int getScore() const;
    
    void saveScore(float duration);
    QList<QPair<int, float>> getTopScores() const;
    void clearLeaderboard();

private:
    int score;
    // Cached leaderboard: avoids QSettings disk I/O on every read
    // (e.g. paintEvent). Invalidated by saveScore/clearLeaderboard.
    mutable QList<QPair<int, float>> topCache;
    mutable bool topCacheValid = false;
};

#endif // SCOREMANAGER_H
