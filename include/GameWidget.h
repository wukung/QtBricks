#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include <QVector>
#include <QRectF>
#include <QElapsedTimer>
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include "ScoreManager.h"

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

    QRectF ballRect() const { return ball.getRect(); }
    QRectF paddleRect() const { return paddle.getRect(); }
    int remainingBricks() const {
        int n = 0;
        for (const Brick &b : bricks) {
            if (!b.isDestroyed()) {
                ++n;
            }
        }
        return n;
    }
    bool isGameStarted() const { return gameStarted; }
    bool isGameOver() const { return gameOver; }
    bool isGameWon() const { return gameWon; }

protected:
    void paintEvent(QPaintEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void initGame();
    void updateGame(float dt);
    void checkCollision();
    void startGameTimer();
    void stopGameTimer();

    int timerId = -1;
    bool gameStarted;
    bool gameOver;
    bool gameWon;
    QElapsedTimer elapsedTimer;
    float gameTime;

    // Game Objects
    Paddle paddle;
    Ball ball;
    QVector<Brick> bricks;
    ScoreManager scoreManager;

    // Constants
    static constexpr int PADDLE_WIDTH = 100;
    static constexpr int PADDLE_HEIGHT = 15;
    static constexpr int BALL_RADIUS = 8;
    static constexpr int BRICK_ROWS = 5;
    static constexpr int BRICK_COLS = 10;
    static constexpr int BRICK_WIDTH = 60;
    static constexpr int BRICK_HEIGHT = 20;
    static constexpr int BRICK_GAP = 5;
    static constexpr int TOP_MARGIN = 50;
    static constexpr int SIDE_MARGIN = 20;
    static constexpr int TIMER_DELAY = 16; // Target ~60 FPS update
    static constexpr float MAX_TIME_STEP = 0.005f; // Max 5ms per physics step
    static constexpr qreal MAX_BALL_SPEED_X = 600.0; // Clamp for paddle bounce
    static constexpr qreal PADDLE_BOUNCE_FACTOR = 8.0; // Hit offset to vx
};

#endif // GAMEWIDGET_H
