#include <QApplication>
#include "GameWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GameWidget gameWindow;
    gameWindow.show();

    return app.exec();
}