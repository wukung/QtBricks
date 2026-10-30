#include "ScoreManager.h"
#include <algorithm>
#include <cmath>

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

void ScoreManager::setSettingsFileOverride(const QString &filePath) {
    fileOverride = filePath;
    topCacheValid = false;
}

void ScoreManager::clearSettingsFileOverride() {
    fileOverride.clear();
    topCacheValid = false;
}

QString ScoreManager::settingsFileOverride() {
    return fileOverride;
}

std::unique_ptr<QSettings> ScoreManager::openSettings() {
    if (!fileOverride.isEmpty()) {
        return std::make_unique<QSettings>(fileOverride, QSettings::IniFormat);
    }
    return std::make_unique<QSettings>("QtBricksDev", "QtBricksGame");
}

void ScoreManager::saveScore(float duration) {
    auto settings = openSettings();

    QList<QPair<int, float>> topScores = getTopScores();
    // The row being saved follows the same validity rules as stored rows.
    if (score >= 0 && duration >= 0 && std::isfinite((double)duration)) {
        topScores.append(qMakePair(score, duration));
    }

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
    settings->beginWriteArray("Leaderboard");
    for (int i = 0; i < topScores.size(); ++i) {
        settings->setArrayIndex(i);
        settings->setValue("score", topScores[i].first);
        settings->setValue("duration", (double)topScores[i].second);
    }
    settings->endArray();
    settings->sync();

    // The board was just written: refresh the shared cache, not invalidate.
    topCache = topScores;
    topCacheValid = true;
}

QList<QPair<int, float>> ScoreManager::getTopScores() const {
    if (topCacheValid) {
        return topCache;
    }

    auto settings = openSettings();
    QList<QPair<int, float>> topScores;

    int size = settings->beginReadArray("Leaderboard");
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        // Skip rows whose values fail conversion instead of coercing to 0:
        // a coerced 0s duration would sort as the best time and evict
        // a real entry from the top 5.
        bool okScore = false, okDuration = false;
        int s = settings->value("score").toInt(&okScore);
        double rawDuration = settings->value("duration").toDouble(&okDuration);
        if (!okScore || !okDuration) {
            continue;
        }
        float d = (float)rawDuration;
        // Drop tampered entries instead of rendering them as-is.
        if (s < 0 || d < 0 || !std::isfinite(d)) {
            continue;
        }
        topScores.append(qMakePair(s, d));
    }
    settings->endArray();

    topCache = topScores;
    topCacheValid = true;
    return topScores;
}

void ScoreManager::clearLeaderboard() {
    auto settings = openSettings();
    settings->remove("Leaderboard");
    settings->sync();
    topCacheValid = false;
}
