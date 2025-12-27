#include "../include/GameWidget.h"
#include <QFont>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <cmath>

GameWidget::GameWidget(QWidget *parent) : QWidget(parent) {
  controller = new GameController();

  // Set fixed size
  setFixedSize(WIDTH, HEIGHT);
  setFocusPolicy(Qt::StrongFocus);

  // Set up game timer (150ms = ~6.7 updates/second)
  gameTimer = new QTimer(this);
  connect(gameTimer, &QTimer::timeout, this, &GameWidget::gameLoop);
  gameTimer->start(150);
}

GameWidget::~GameWidget() { delete controller; }

void GameWidget::gameLoop() {
  controller->update();
  update(); // Trigger repaint
}

void GameWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // 1. Clear background
  painter.fillRect(0, 0, WIDTH, HEIGHT, Qt::black);

  // 2. Draw Maze (Walls)
  painter.setBrush(QColor(33, 33, 222)); // Wall Blue
  painter.setPen(Qt::NoPen);
  
  for (int x = 0; x < Graph::WIDTH; x++) {
      for (int y = 0; y < Graph::HEIGHT; y++) {
          if (Graph::isWall(x, y)) {
              painter.drawRect(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
          }
      }
  }

  // 3. Draw Pacman
  Pacman *p = controller->getPacman();
  Location pLoc = p->getLocation();
  Location pDir = p->getLastDirection();
  
  // Animation logic: Mouth opens/closes every ~300ms
  // Use current time or a simple static counter if available, or just system clock
  // Since we don't have a frame counter, we can use the system clock
  auto now = std::chrono::steady_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
  double mouthOpen = 30.0 * (0.5 + 0.5 * std::sin(ms * 0.015)); // Oscillate between 0 and 30 degrees

  painter.setBrush(Qt::yellow);
  painter.setPen(Qt::NoPen);

  int px = pLoc.x * TILE_SIZE;
  int py = pLoc.y * TILE_SIZE;
  
  // Rotation Logic
  int startAngle = 0;
  if (pDir.x == 1) startAngle = 0;
  else if (pDir.x == -1) startAngle = 180;
  else if (pDir.y == -1) startAngle = 90;
  else if (pDir.y == 1) startAngle = 270;

  // drawPie uses 1/16th of a degree
  // We want the mouth to center on the direction. 
  // e.g. Right (0): Mouth from 30 to 330 (Viewed as 30 start, -300 span? Or 30 start + 300 span)
  // Qt Angles: 3 o'clock is 0. Positive is CCW.
  // Mouth Opening 30 means +/- 15 degrees from center? Or 30 degrees total wedge?
  // Let's say Total Wedge is 'mouthOpen * 2'.
  // Start = DirectionAngle + mouthOpen
  // Span = 360 - 2 * mouthOpen
  
  int qtStartAngle = (startAngle + (int)mouthOpen) * 16;
  int qtSpanAngle = (360 - 2 * (int)mouthOpen) * 16;
  
  painter.drawPie(px, py, TILE_SIZE, TILE_SIZE, qtStartAngle, qtSpanAngle);


  // 4. Draw Monsters
  const auto &monsters = controller->getMonsters();
  for (Monster *monster : monsters) {
    Location mLoc = monster->getLocation();
    std::string name = monster->getName();
    int mx = mLoc.x * TILE_SIZE;
    int my = mLoc.y * TILE_SIZE;

    // Set color
    if (name == "M1 (Dist)") painter.setBrush(Qt::red);
    else if (name == "M2 (Heur)") painter.setBrush(QColor(255, 184, 255)); // Pink
    else if (name == "M3 (Dir)") painter.setBrush(Qt::cyan);
    else if (name == "M4 (Aggr)") painter.setBrush(QColor(255, 184, 82)); // Orange
    else painter.setBrush(Qt::green);

    // Draw Ghost Body (Circle top, Rect bottom, Wavy feet)
    QPainterPath path;
    
    // Head (Top Half Circle)
    path.moveTo(mx, my + TILE_SIZE / 2);
    path.arcTo(mx, my, TILE_SIZE, TILE_SIZE, 180, 180);
    
    // Feet (Wavy)
    // 3 small bumps
    double footW = TILE_SIZE / 3.0;
    // Right side down
    path.lineTo(mx + TILE_SIZE, my + TILE_SIZE);
    
    // Bottom bumps (Right to Left)
    path.lineTo(mx + 2 * footW, my + TILE_SIZE - 4);
    path.lineTo(mx + 1 * footW, my + TILE_SIZE);
    path.lineTo(mx, my + TILE_SIZE - 4);
    
    path.closeSubpath();
    
    painter.drawPath(path);

    // Eyes (White) - Fixed looking leftish for now
    painter.setBrush(Qt::white);
    int eyeSize = TILE_SIZE / 3;
    int eyeY = my + TILE_SIZE / 4;
    painter.drawEllipse(mx + 4, eyeY, eyeSize, eyeSize);
    painter.drawEllipse(mx + TILE_SIZE - 4 - eyeSize, eyeY, eyeSize, eyeSize);

    // Pupils (Blue)
    painter.setBrush(Qt::blue);
    int pupilSize = eyeSize / 2;
    painter.drawEllipse(mx + 4 + 2, eyeY + 2, pupilSize, pupilSize);
    painter.drawEllipse(mx + TILE_SIZE - 4 - eyeSize + 2, eyeY + 2, pupilSize, pupilSize);
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
  default:
    return;
  }

  controller->handleInput(key);
}

GameController *GameWidget::getController() const { return controller; }
