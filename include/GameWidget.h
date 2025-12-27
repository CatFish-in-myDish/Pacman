#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "GameController.h"
#include <QTimer>
#include <QWidget>

class GameWidget : public QWidget {
  Q_OBJECT

private:
  static const int TILE_SIZE = 15;
  static const int WIDTH = Graph::WIDTH * TILE_SIZE;
  static const int HEIGHT = Graph::HEIGHT * TILE_SIZE;

  GameController *controller;
  QTimer *gameTimer;

protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

private slots:
  void gameLoop();

public:
  GameWidget(QWidget *parent = nullptr);
  ~GameWidget();

  GameController *getController() const;
};

#endif
