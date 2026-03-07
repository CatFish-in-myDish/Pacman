/**
 * Handles the rendering of the game state using Qt's QPainter.
 *
 * This class draws the maze, Pacman, monsters, pellets, and other visual
 * elements. It also handles keyboard input for controlling Pacman.
 *
 */
#include "../include/GameWidget.h"
#include <QFont>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <cmath>

GameWidget::GameWidget(QWidget *parent) : QWidget(parent) {
  controller = new GameController();

  setFixedSize(WIDTH, HEIGHT);
  setFocusPolicy(Qt::StrongFocus);

  gameTimer = new QTimer(this);
  connect(gameTimer, &QTimer::timeout, this, &GameWidget::gameLoop);
  gameTimer->start(250);
}

GameWidget::~GameWidget() { delete controller; }

void GameWidget::gameLoop() {
  controller->update();
  update();
}

void GameWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // 1. Clear background
  painter.fillRect(0, 0, WIDTH, HEIGHT, Qt::black);

  // 2. Draw Maze (Walls)
  painter.setBrush(QColor(33, 33, 222));
  painter.setPen(Qt::NoPen);
  for (int x = 0; x < Graph::WIDTH; x++) {
    for (int y = 0; y < Graph::HEIGHT; y++) {
      if (Graph::isWall(x, y)) {
        int px = x * TILE_SIZE;
        int py = y * TILE_SIZE;
        int r = TILE_SIZE / 3;

        painter.drawRoundedRect(px, py, TILE_SIZE, TILE_SIZE, r, r);

        if (Graph::isWall(x, y - 1)) {
          painter.drawRect(px, py, TILE_SIZE, r);
        }
        if (Graph::isWall(x, y + 1)) {
          painter.drawRect(px, py + TILE_SIZE - r, TILE_SIZE, r);
        }
        if (Graph::isWall(x - 1, y)) {
          painter.drawRect(px, py, r, TILE_SIZE);
        }
        if (Graph::isWall(x + 1, y)) {
          painter.drawRect(px + TILE_SIZE - r, py, r, TILE_SIZE);
        }
      }
    }
  }

  // 2.5 Draw Pellets
  painter.setBrush(Qt::white);
  painter.setPen(Qt::NoPen);
  const auto &pellets = controller->getPellets();
  int pelletRadius = TILE_SIZE / 8;
  for (const Location &loc : pellets) {
    int cx = loc.x * TILE_SIZE + TILE_SIZE / 2;
    int cy = loc.y * TILE_SIZE + TILE_SIZE / 2;
    painter.drawEllipse(cx - pelletRadius, cy - pelletRadius, pelletRadius * 2,
                        pelletRadius * 2);
  }

  // 3. Draw Pacman
  Pacman *p = controller->getPacman();
  Location pLoc = p->getLocation();
  Location pDir = p->getLastDirection();

  auto now = std::chrono::steady_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch())
                .count();
  double mouthOpen = 30.0 * (0.5 + 0.5 * std::sin(ms * 0.015));

  painter.setBrush(Qt::yellow);
  painter.setPen(Qt::NoPen);

  int px = pLoc.x * TILE_SIZE;
  int py = pLoc.y * TILE_SIZE;

  int startAngle = 0;
  if (pDir.x == 1)
    startAngle = 0;
  else if (pDir.x == -1)
    startAngle = 180;
  else if (pDir.y == -1)
    startAngle = 90;
  else if (pDir.y == 1)
    startAngle = 270;

  painter.drawPie(px, py, TILE_SIZE, TILE_SIZE,
                  (startAngle + (int)mouthOpen) * 16,
                  (360 - 2 * (int)mouthOpen) * 16);

  // 4. Draw Monsters
  const auto &monsters = controller->getMonsters();
  for (Monster *monster : monsters) {
    Location mLoc = monster->getLocation();
    std::string name = monster->getName();
    int mx = mLoc.x * TILE_SIZE;
    int my = mLoc.y * TILE_SIZE;

    // ── Pick ghost body colour ────────────────────────────────────────────
    QColor ghostColour;
    if (name == "M1 (Dist)")
      ghostColour = QColor(255, 80, 80); // Bright Red
    else if (name == "M2 (Heur)")
      ghostColour = QColor(255, 182, 255); // Light Magenta
    else if (name == "M3 (Dir)")
      ghostColour = QColor(102, 255, 255); // Light Cyan
    else if (name == "M4 (Aggr)")
      ghostColour = QColor(255, 184, 82); // Orange
    else if (name == "M5 (Pinch)")
      ghostColour = QColor(186, 255, 201); // Mint Green
    else if (name == "M6 (Random)")
      ghostColour = QColor(255, 128, 0); // Dark Orange
    else
      ghostColour = QColor(255, 255, 153); // Soft Neon Yellow

    painter.setPen(Qt::NoPen);

    // ── Ghost body ────────────────────────────────────────────────────────
    // Classic Pac-Man shape:
    //   • Rounded dome (top half-circle arc)
    //   • Straight sides
    //   • Three wavy bumps along the bottom skirt
    // ─────────────────────────────────────────────────────────────────────
    double bw = TILE_SIZE / 3.0;
    QPainterPath bodyPath;

    // Dome: start at left edge of dome midpoint, sweep 180° CW to right edge
    bodyPath.moveTo(mx, my + TILE_SIZE / 2);
    bodyPath.arcTo(mx, my, TILE_SIZE, TILE_SIZE, 180, 180);

    // Right side straight down to bottom-right corner
    bodyPath.lineTo(mx + TILE_SIZE, my + TILE_SIZE);

    // Three wavy bumps, drawn right-to-left:
    //   alternating upward bump arc then downward valley arc
    bodyPath.arcTo(mx + 2 * bw, my + TILE_SIZE - bw, bw, bw, 0,
                   -180); // right bump
    bodyPath.arcTo(mx + 1 * bw, my + TILE_SIZE - bw, bw, bw, 0,
                   180);                                      // middle valley
    bodyPath.arcTo(mx, my + TILE_SIZE - bw, bw, bw, 0, -180); // left bump

    bodyPath.lineTo(mx, my + TILE_SIZE / 2);
    bodyPath.closeSubpath();

    painter.setBrush(ghostColour);
    painter.fillPath(bodyPath, ghostColour);

    // ── Eyes: large white sockets + vivid blue pupils ─────────────────────
    int eyeOuterW = TILE_SIZE / 3;
    int eyeOuterH = (int)(TILE_SIZE * 0.38);
    int eyeY = my + TILE_SIZE / 6;
    int leftEyeX = mx + TILE_SIZE / 6;
    int rightEyeX = mx + TILE_SIZE - TILE_SIZE / 6 - eyeOuterW;

    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(leftEyeX, eyeY, eyeOuterW, eyeOuterH);
    painter.drawEllipse(rightEyeX, eyeY, eyeOuterW, eyeOuterH);

    int pupilW = eyeOuterW / 2;
    int pupilH = eyeOuterH / 2;
    int pupilOffX = eyeOuterW / 4;
    int pupilOffY = eyeOuterH / 4;

    painter.setBrush(QColor(0, 80, 255));
    painter.drawEllipse(leftEyeX + pupilOffX, eyeY + pupilOffY, pupilW, pupilH);
    painter.drawEllipse(rightEyeX + pupilOffX, eyeY + pupilOffY, pupilW,
                        pupilH);

    // ── Slowed indicator ring ─────────────────────────────────────────────
    if (monster->isSlowed()) {
      painter.setPen(QPen(Qt::white, 2));
      painter.setBrush(Qt::NoBrush);
      painter.drawEllipse(mx - 2, my - 2, TILE_SIZE + 4, TILE_SIZE + 4);
      painter.setPen(Qt::NoPen);
    }

    // ── Frightened indicator: replace eyes with scared expression ─────────
    if (monster->isFrightened()) {
      // Draw wavy scared mouth
      painter.setPen(QPen(Qt::white, 1));
      int mouthY = my + TILE_SIZE * 2 / 3;
      int mouthLeft = mx + TILE_SIZE / 5;
      int mouthRight = mx + TILE_SIZE * 4 / 5;
      int segments = 4;
      double segW = (double)(mouthRight - mouthLeft) / segments;
      for (int s = 0; s < segments; ++s) {
        int x1i = mouthLeft + (int)(s * segW);
        int x2i = mouthLeft + (int)((s + 1) * segW);
        int yOff = (s % 2 == 0) ? -1 : 1;
        painter.drawLine(x1i, mouthY, x2i, mouthY + yOff * 2);
      }
      painter.setPen(Qt::NoPen);
    }
  }

  // 5.5 Draw Power Pellets (larger, blinking)
  const auto &powerPellets = controller->getPowerPellets();
  int powerRadius = TILE_SIZE * 0.4;
  bool blink = ((ms / 250) % 2 == 0);
  if (blink) {
    painter.setBrush(Qt::red); // Red
    painter.setPen(Qt::NoPen);
    for (const Location &loc : powerPellets) {
      int cx = loc.x * TILE_SIZE + TILE_SIZE / 2;
      int cy = loc.y * TILE_SIZE + TILE_SIZE / 2;
      painter.drawEllipse(cx - powerRadius, cy - powerRadius, powerRadius * 2,
                          powerRadius * 2);
    }
  }

  // 6. Draw Lightning Bolt
  if (controller->isLightningActive()) {
    auto arc = controller->getLightningArc();
    int x1 = arc.first.x * TILE_SIZE + TILE_SIZE / 2;
    int y1 = arc.first.y * TILE_SIZE + TILE_SIZE / 2;
    int x2 = arc.second.x * TILE_SIZE + TILE_SIZE / 2;
    int y2 = arc.second.y * TILE_SIZE + TILE_SIZE / 2;

    QPen boltPen(QColor(100, 200, 255), 4);
    boltPen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(boltPen);
    painter.drawLine(x1, y1, x2, y2);
    painter.setPen(QPen(Qt::white, 2));
    painter.drawLine(x1, y1, x2, y2);
  }

  // HUD
  painter.setPen(Qt::white);
  painter.setFont(QFont("Arial", 14, QFont::Bold));
  painter.drawText(8, 20, QString("Score: %1").arg(controller->getScore()));
  painter.drawText(120, 20, QString("Round: %1").arg(controller->getRound()));

  // Win / Game Over overlay
  if (controller->isGameWon()) {
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 160));
    painter.drawRect(0, 0, WIDTH, HEIGHT);
    painter.setPen(Qt::green);
    painter.setFont(QFont("Arial", 48, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, QString("YOU WIN!"));
  } else if (controller->isGameOver()) {
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 160));
    painter.drawRect(0, 0, WIDTH, HEIGHT);
    painter.setPen(Qt::red);
    painter.setFont(QFont("Arial", 48, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, QString("GAME OVER"));
  }
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
  QString key;
  switch (event->key()) {
  case Qt::Key_Up:
    key = "UP";
    break;
  case Qt::Key_Down:
    key = "DOWN";
    break;
  case Qt::Key_Left:
    key = "LEFT";
    break;
  case Qt::Key_Right:
    key = "RIGHT";
    break;
  case Qt::Key_R:
    key = "R";
    break;
  case Qt::Key_Z:
    key = "Z";
    break;
  default:
    return;
  }
  controller->handleInput(key);
}

GameController *GameWidget::getController() const { return controller; }
