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
};

#endif // SCOREMANAGER_H
