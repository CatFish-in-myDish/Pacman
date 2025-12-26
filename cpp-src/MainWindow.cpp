#include "MainWindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Graph Pacman Survival");

    // Create central widget with vertical layout
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Create status label
    statusLabel = new QLabel("Time Survived: 0.0s | Use Arrow Keys to Start | R to Restart");
    statusLabel->setStyleSheet("QLabel { font-size: 14px; padding: 10px; background-color: #333; color: white; }");
    statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(statusLabel);

    // Create game widget
    gameWidget = new GameWidget();
    layout->addWidget(gameWidget);

    setCentralWidget(centralWidget);

    // Set window to fixed size
    setFixedSize(sizeHint());

    // Set up timer to update status label
    statusTimer = new QTimer(this);
    connect(statusTimer, &QTimer::timeout, this, &MainWindow::updateStatus);
    statusTimer->start(100); // Update status 10 times per second
}

void MainWindow::updateStatus() {
    GameController* controller = gameWidget->getController();

    if (controller->isGameOver()) {
        statusLabel->setText(QString("GAME OVER! Final Time: %1s | Press R to Restart")
                            .arg(controller->getSurvivedTime(), 0, 'f', 2));
    } else {
        statusLabel->setText(QString("Time Survived: %1s")
                            .arg(controller->getSurvivedTime(), 0, 'f', 2));
    }
}
