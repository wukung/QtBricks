#include "GameWidget.h"
#include <QApplication>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    
    int windowWidth = SIDE_MARGIN * 2 + BRICK_COLS * BRICK_WIDTH + (BRICK_COLS - 1) * BRICK_GAP;
    int windowHeight = 600;
    setFixedSize(windowWidth, windowHeight);
    
    initGame();
}

GameWidget::~GameWidget()
{
}

void GameWidget::initGame()
{
    gameStarted = false;
    gameOver = false;
    gameWon = false;
    gameTime = 0.0f;
    
    scoreManager.reset();

    // Init paddle
    qreal paddleStartX = width() / 2.0 - PADDLE_WIDTH / 2.0;
    qreal paddleStartY = height() - 40;
    paddle.init(paddleStartX, paddleStartY, PADDLE_WIDTH, PADDLE_HEIGHT, 450.0);

    // Init ball
    qreal ballStartX = width() / 2.0 - BALL_RADIUS;
    qreal ballStartY = paddleStartY - BALL_RADIUS * 2 - 1;
    ball.init(ballStartX, ballStartY, BALL_RADIUS, 250.0, -250.0);

    // Init bricks
    bricks.clear();
    for (int row = 0; row < BRICK_ROWS; ++row) {
        for (int col = 0; col < BRICK_COLS; ++col) {
            Brick brick;
            qreal x = SIDE_MARGIN + col * (BRICK_WIDTH + BRICK_GAP);
            qreal y = TOP_MARGIN + row * (BRICK_HEIGHT + BRICK_GAP);
            
            QColor brickColor;
            switch(row % 5) {
                case 0: brickColor = QColor(255, 100, 100); break;
                case 1: brickColor = QColor(255, 150, 100); break;
                case 2: brickColor = QColor(255, 255, 100); break;
                case 3: brickColor = QColor(100, 255, 100); break;
                case 4: brickColor = QColor(100, 100, 255); break;
            }
            
            brick.init(x, y, BRICK_WIDTH, BRICK_HEIGHT, brickColor);
            bricks.append(brick);
        }
    }

    elapsedTimer.start();
    timerId = startTimer(TIMER_DELAY);
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fill background
    painter.fillRect(rect(), QColor(30, 30, 30));

    // Draw score
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(10, 30, QString("Score: %1").arg(scoreManager.getScore()));

    auto drawLeaderboard = [&](const QString& title) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        
        QRect titleRect(0, 0, width(), height() / 2 - 50);
        painter.drawText(titleRect, Qt::AlignBottom | Qt::AlignHCenter, title + "\nPress Space to Restart");
        
        painter.setFont(QFont("Arial", 16));
        auto top = scoreManager.getTopScores();
        QString topStr = "Top 5:\n";
        for (int i=0; i<top.size(); ++i) {
            topStr += QString("%1. %2 pts (%3s)\n").arg(i+1).arg(top[i].first).arg(top[i].second, 0, 'f', 1);
        }
        
        QRect boardRect(0, height() / 2 - 20, width(), height() / 2 + 20);
        painter.drawText(boardRect, Qt::AlignTop | Qt::AlignHCenter, topStr);
    };

    if (gameOver) {
        drawLeaderboard("Game Over");
        return;
    }

    if (gameWon) {
        drawLeaderboard("You Win!");
        return;
    }

    // Draw objects
    paddle.draw(&painter);
    ball.draw(&painter);
    for (int i = 0; i < bricks.size(); ++i) {
        bricks[i].draw(&painter);
    }

    if (!gameStarted) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 14));
        painter.drawText(rect(), Qt::AlignCenter, "Press Space to Start");
    }
}

void GameWidget::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerId) {
        float dt = elapsedTimer.restart() / 1000.0f;
        if (dt > 0.1f) dt = 0.1f;
        
        if (gameStarted && !gameOver && !gameWon) {
            updateGame(dt);
            repaint();
        }
    } else {
        QWidget::timerEvent(event);
    }
}

void GameWidget::updateGame(float dt)
{
    float timeRemaining = dt;
    gameTime += dt;
    
    while (timeRemaining > 0) {
        float step = qMin(timeRemaining, MAX_TIME_STEP);
        timeRemaining -= step;

        paddle.update(step, width());
        ball.move(step);

        QRectF bRect = ball.getRect();

        // Check boundaries
        if (bRect.left() <= 0) {
            ball.setSpeedX(qAbs(ball.getSpeedX()));
            ball.moveLeft(0);
        }
        if (bRect.right() >= width()) {
            ball.setSpeedX(-qAbs(ball.getSpeedX()));
            ball.moveRight(width());
        }
        if (bRect.top() <= 0) {
            ball.setSpeedY(qAbs(ball.getSpeedY()));
            ball.moveTop(0);
        }
        
        // Bottom boundary - Game Over
        if (bRect.bottom() >= height()) {
            if (!gameOver) {
                gameOver = true;
                scoreManager.saveScore(gameTime);
                break; // 停止後續的物理子步進與碰撞判定
            }
        }

        checkCollision();
        
        if (gameWon) {
            break; // 若已過關則停止後續子步進
        }
    }
}

void GameWidget::checkCollision()
{
    QRectF bRect = ball.getRect();
    QRectF pRect = paddle.getRect();

    // Ball with paddle
    if (bRect.intersects(pRect)) {
        float overlapTop = bRect.bottom() - pRect.top();
        float overlapBottom = pRect.bottom() - bRect.top();
        float overlapLeft = bRect.right() - pRect.left();
        float overlapRight = pRect.right() - bRect.left();

        float minOverlap = qMin(qMin(overlapTop, overlapBottom), qMin(overlapLeft, overlapRight));

        if (minOverlap == overlapTop) {
            ball.moveBottom(pRect.top() - 0.1f);
            ball.setSpeedY(-qAbs(ball.getSpeedY()));

            qreal paddleCenter = pRect.left() + pRect.width() / 2.0;
            qreal ballCenter = ball.getRect().left() + ball.getRect().width() / 2.0;
            ball.setSpeedX((ballCenter - paddleCenter) * 8.0); 
            
            if (qAbs(ball.getSpeedY()) < 100.0) {
                ball.setSpeedY(-100.0);
            }
        } else if (minOverlap == overlapLeft) {
            ball.moveRight(pRect.left() - 0.1f);
            ball.setSpeedX(-qAbs(ball.getSpeedX()));
        } else if (minOverlap == overlapRight) {
            ball.moveLeft(pRect.right() + 0.1f);
            ball.setSpeedX(qAbs(ball.getSpeedX()));
        } else {
            ball.moveTop(pRect.bottom() + 0.1f);
            ball.setSpeedY(qAbs(ball.getSpeedY()));
        }
        
        // Update local reference
        bRect = ball.getRect();
    }

    // Ball with bricks
    for (int i = 0; i < bricks.size(); ++i) {
        if (!bricks[i].isDestroyed()) {
            QRectF brickRect = bricks[i].getRect();
            if (bRect.intersects(brickRect)) {
                bricks[i].setDestroyed(true);
                int row = i / BRICK_COLS;
                int points = (BRICK_ROWS - row) * 10;
                scoreManager.addScore(points);
                
                float overlapTop = bRect.bottom() - brickRect.top();
                float overlapBottom = brickRect.bottom() - bRect.top();
                float overlapLeft = bRect.right() - brickRect.left();
                float overlapRight = brickRect.right() - bRect.left();

                float minOverlap = qMin(qMin(overlapTop, overlapBottom), qMin(overlapLeft, overlapRight));

                if (minOverlap == overlapTop) {
                    ball.moveBottom(brickRect.top() - 0.1f);
                    ball.setSpeedY(-qAbs(ball.getSpeedY()));
                } else if (minOverlap == overlapBottom) {
                    ball.moveTop(brickRect.bottom() + 0.1f);
                    ball.setSpeedY(qAbs(ball.getSpeedY()));
                } else if (minOverlap == overlapLeft) {
                    ball.moveRight(brickRect.left() - 0.1f);
                    ball.setSpeedX(-qAbs(ball.getSpeedX()));
                } else if (minOverlap == overlapRight) {
                    ball.moveLeft(brickRect.right() + 0.1f);
                    ball.setSpeedX(qAbs(ball.getSpeedX()));
                }
                
                break; // Only break one brick per step
            }
        }
    }

    bool allDestroyed = true;
    for (int i = 0; i < bricks.size(); ++i) {
        if (!bricks[i].isDestroyed()) {
            allDestroyed = false;
            break;
        }
    }

    if (allDestroyed) {
        if (!gameWon) {
            gameWon = true;
            scoreManager.saveScore(gameTime);
        }
    }
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Left:
            paddle.setMovingLeft(true);
            break;
        case Qt::Key_Right:
            paddle.setMovingRight(true);
            break;
        case Qt::Key_Space:
            if (gameOver || gameWon) {
                killTimer(timerId);
                initGame();
                repaint();
            } else if (!gameStarted) {
                gameStarted = true;
            }
            break;
        case Qt::Key_Escape:
            QApplication::quit();
            break;
        default:
            QWidget::keyPressEvent(event);
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Left:
            paddle.setMovingLeft(false);
            break;
        case Qt::Key_Right:
            paddle.setMovingRight(false);
            break;
        default:
            QWidget::keyReleaseEvent(event);
    }
}
