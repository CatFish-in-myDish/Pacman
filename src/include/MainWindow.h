#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "GameWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    GameWidget* gameWidget;
    QLabel* statusLabel;
    QTimer* statusTimer;

private slots:
    void updateStatus();

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;
};

#endif 