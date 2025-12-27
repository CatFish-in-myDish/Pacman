#include "../include/GameWidget.h"
#include <QFont>
#include <QKeyEvent>
#include <QPainter>

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

  // Clear background
  painter.fillRect(0, 0, WIDTH, HEIGHT, Qt::black);

  // Draw Pacman
  Pacman *p = controller->getPacman();
  Location pLoc = p->getLocation();
  painter.setBrush(Qt::yellow);
  painter.drawEllipse(pLoc.x * TILE_SIZE, pLoc.y * TILE_SIZE, TILE_SIZE,
                      TILE_SIZE);

  // Draw 'P' on Pacman
  painter.setPen(Qt::black);
  QFont font = painter.font();
  font.setPointSize(8);
  font.setBold(true);
  painter.setFont(font);
  painter.drawText(pLoc.x * TILE_SIZE + 4, pLoc.y * TILE_SIZE + 11, "P");

  // Draw Monsters
  const auto &monsters = controller->getMonsters();
  for (Monster *monster : monsters) {
    Location mLoc = monster->getLocation();
    std::string name = monster->getName();

    // Set color based on monster type
    if (name == "M1 (Dist)") {
      painter.setBrush(Qt::red);
    } else if (name == "M2 (Heur)") {
      painter.setBrush(QColor(255, 192, 203)); // Pink
    } else if (name == "M3 (Dir)") {
      painter.setBrush(Qt::cyan);
    } else if (name == "M4 (Aggr)") {
      painter.setBrush(QColor(255, 165, 0)); // Orange
    } else {
      painter.setBrush(Qt::green);
    }

    painter.drawRect(mLoc.x * TILE_SIZE, mLoc.y * TILE_SIZE, TILE_SIZE,
                     TILE_SIZE);
  }

  // Draw grid lines (optional, subtle)
  painter.setPen(QPen(Qt::darkGray, 0.5));
  for (int x = 0; x <= Graph::WIDTH; x++) {
    painter.drawLine(x * TILE_SIZE, 0, x * TILE_SIZE, HEIGHT);
  }
  for (int y = 0; y <= Graph::HEIGHT; y++) {
    painter.drawLine(0, y * TILE_SIZE, WIDTH, y * TILE_SIZE);
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
