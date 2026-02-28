/**
 * Entry point of the Pacman application.
 *
 * Initialises the Qt application and the main window.
 *
 * Time Complexity: O(1) startup.
 */
#include "../include/MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MainWindow window;
  window.show();

  return app.exec();
}
