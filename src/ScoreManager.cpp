#include "ScoreManager.h"
#include <algorithm>

ScoreManager::ScoreManager() : score(0) {}

void ScoreManager::reset() {
    score = 0;
}

void ScoreManager::addScore(int points) {
    if (points > 0) {
        score += points;
    }
}

int ScoreManager::getScore() const {
    return score;
}

void ScoreManager::saveScore(float duration) {
    QSettings settings("QtBricksDev", "QtBricksGame");
    
    QList<QPair<int, float>> topScores = getTopScores();
    topScores.append(qMakePair(score, duration));
    
    // Sort: score descending, then duration ascending
    std::sort(topScores.begin(), topScores.end(), [](const QPair<int, float>& a, const QPair<int, float>& b) {
        if (a.first != b.first) {
            return a.first > b.first;
        }
        return a.second < b.second;
    });
    
    // Keep top 5
    if (topScores.size() > 5) {
        topScores = topScores.mid(0, 5);
    }
    
    // Save back
    settings.beginWriteArray("Leaderboard");
    for (int i = 0; i < topScores.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("score", topScores[i].first);
        settings.setValue("duration", (double)topScores[i].second);
    }
    settings.endArray();
    settings.sync();
}

QList<QPair<int, float>> ScoreManager::getTopScores() const {
    QSettings settings("QtBricksDev", "QtBricksGame");
    QList<QPair<int, float>> topScores;
    
    int size = settings.beginReadArray("Leaderboard");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        int s = settings.value("score").toInt();
        float d = (float)settings.value("duration").toDouble();
        topScores.append(qMakePair(s, d));
    }
    settings.endArray();
    
    return topScores;
}

void ScoreManager::clearLeaderboard() {
    QSettings settings("QtBricksDev", "QtBricksGame");
    settings.remove("Leaderboard");
    settings.sync();
}
