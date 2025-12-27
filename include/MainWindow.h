#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GameWidget.h"
#include <QLabel>
#include <QMainWindow>

class MainWindow : public QMainWindow {
  Q_OBJECT

private:
  GameWidget *gameWidget;
  QLabel *statusLabel;
  QTimer *statusTimer;

private slots:
  void updateStatus();

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow() = default;
};

#endif
