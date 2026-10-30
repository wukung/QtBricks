#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <QSettings>
#include <QList>
#include <QPair>
#include <memory>

class ScoreManager {
public:
    ScoreManager();

    void reset();
    void addScore(int points);
    int getScore() const;

    void saveScore(float duration);
    QList<QPair<int, float>> getTopScores() const;
    void clearLeaderboard();

    // Test-only: redirect the backing store to an explicit ini file so
    // tests never touch the user's real leaderboard on any OS.
    // Production never sets this and keeps NativeFormat storage.
    static void setSettingsFileOverride(const QString &filePath);
    static void clearSettingsFileOverride();
    static QString settingsFileOverride();

private:
    static std::unique_ptr<QSettings> openSettings();

    int score;
    // Process-wide leaderboard cache: the board lives in QSettings, so
    // every writer (any instance) refreshes the single shared cache.
    // Avoids disk I/O on every read (e.g. paintEvent).
    inline static QList<QPair<int, float>> topCache;
    inline static bool topCacheValid = false;
    inline static QString fileOverride;
};

#endif // SCOREMANAGER_H
