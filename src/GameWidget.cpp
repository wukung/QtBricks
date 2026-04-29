#include "GameWidget.h"
#include <QApplication>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
{
    // Make sure widget can receive keyboard focus
    setFocusPolicy(Qt::StrongFocus);
    
    // Set fixed size for the game window
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
    moveLeft = false;
    moveRight = false;

    // Init paddle
    paddleDx = 7.0;
    paddle.setRect(width() / 2.0 - PADDLE_WIDTH / 2.0, height() - 40, PADDLE_WIDTH, PADDLE_HEIGHT);

    // Init ball
    ballDx = 4.0;
    ballDy = -4.0;
    ball.setRect(width() / 2.0 - BALL_RADIUS, paddle.top() - BALL_RADIUS * 2 - 1, BALL_RADIUS * 2, BALL_RADIUS * 2);

    // Init bricks
    bricks.clear();
    for (int row = 0; row < BRICK_ROWS; ++row) {
        for (int col = 0; col < BRICK_COLS; ++col) {
            Brick brick;
            qreal x = SIDE_MARGIN + col * (BRICK_WIDTH + BRICK_GAP);
            qreal y = TOP_MARGIN + row * (BRICK_HEIGHT + BRICK_GAP);
            brick.rect.setRect(x, y, BRICK_WIDTH, BRICK_HEIGHT);
            brick.destroyed = false;
            bricks.append(brick);
        }
    }

    timerId = startTimer(TIMER_DELAY);
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fill background
    painter.fillRect(rect(), QColor(30, 30, 30));

    if (gameOver) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "Game Over\nPress Space to Restart");
        return;
    }

    if (gameWon) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "You Win!\nPress Space to Restart");
        return;
    }

    // Draw Paddle
    painter.setBrush(QColor(100, 200, 255));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(paddle, 5, 5);

    // Draw Ball
    painter.setBrush(QColor(255, 100, 100));
    painter.drawEllipse(ball);

    // Draw Bricks
    for (int i = 0; i < bricks.size(); ++i) {
        if (!bricks[i].destroyed) {
            // Give different colors based on row
            int row = i / BRICK_COLS;
            QColor brickColor;
            switch(row % 5) {
                case 0: brickColor = QColor(255, 100, 100); break;
                case 1: brickColor = QColor(255, 150, 100); break;
                case 2: brickColor = QColor(255, 255, 100); break;
                case 3: brickColor = QColor(100, 255, 100); break;
                case 4: brickColor = QColor(100, 100, 255); break;
            }
            painter.setBrush(brickColor);
            painter.drawRoundedRect(bricks[i].rect, 3, 3);
        }
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
        if (gameStarted && !gameOver && !gameWon) {
            updateGame();
            repaint();
        }
    } else {
        QWidget::timerEvent(event);
    }
}

void GameWidget::updateGame()
{
    // Move paddle
    if (moveLeft && paddle.left() > 0) {
        paddle.translate(-paddleDx, 0);
    }
    if (moveRight && paddle.right() < width()) {
        paddle.translate(paddleDx, 0);
    }

    // Move ball
    ball.translate(ballDx, ballDy);

    // Check boundaries
    if (ball.left() <= 0) {
        ballDx = qAbs(ballDx);
        ball.moveLeft(0);
    }
    if (ball.right() >= width()) {
        ballDx = -qAbs(ballDx);
        ball.moveRight(width());
    }
    if (ball.top() <= 0) {
        ballDy = qAbs(ballDy);
        ball.moveTop(0);
    }
    
    // Bottom boundary - Game Over
    if (ball.bottom() >= height()) {
        gameOver = true;
    }

    checkCollision();
}

void GameWidget::checkCollision()
{
    // Ball with paddle
    if (ball.intersects(paddle)) {
        // Simple physics: bounce based on where it hit the paddle
        qreal paddleCenter = paddle.left() + paddle.width() / 2.0;
        qreal ballCenter = ball.left() + ball.width() / 2.0;
        
        ballDx = (ballCenter - paddleCenter) * 0.15;
        ballDy = -qAbs(ballDy); // Always bounce up
        
        // Ensure minimum vertical speed
        if (qAbs(ballDy) < 2.0) {
            ballDy = -2.0;
        }
        
        // Push ball above paddle to prevent getting stuck
        ball.moveBottom(paddle.top() - 1);
    }

    // Ball with bricks
    bool allDestroyed = true;
    for (int i = 0; i < bricks.size(); ++i) {
        if (!bricks[i].destroyed) {
            allDestroyed = false;
            
            if (ball.intersects(bricks[i].rect)) {
                bricks[i].destroyed = true;
                
                // Determine bounce direction
                QRectF b = bricks[i].rect;
                
                bool hitFromBottom = ball.top() < b.bottom() && ball.bottom() > b.bottom() && ballDy < 0;
                bool hitFromTop = ball.bottom() > b.top() && ball.top() < b.top() && ballDy > 0;
                bool hitFromLeft = ball.right() > b.left() && ball.left() < b.left() && ballDx > 0;
                bool hitFromRight = ball.left() < b.right() && ball.right() > b.right() && ballDx < 0;
                
                if (hitFromTop || hitFromBottom) {
                    ballDy = -ballDy;
                } else if (hitFromLeft || hitFromRight) {
                    ballDx = -ballDx;
                } else {
                    // Fallback
                    ballDy = -ballDy;
                }
                
                break; // Only break one brick per frame
            }
        }
    }

    if (allDestroyed) {
        gameWon = true;
    }
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Left:
            moveLeft = true;
            break;
        case Qt::Key_Right:
            moveRight = true;
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
            moveLeft = false;
            break;
        case Qt::Key_Right:
            moveRight = false;
            break;
        default:
            QWidget::keyReleaseEvent(event);
    }
}
