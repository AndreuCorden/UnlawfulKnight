#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "src/GameController.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stackedWidget;
    GameController *m_gameController;

    // Screen setup methods
    QWidget *createMainMenuScreen();
    QWidget *createSaveSelectScreen();
    QWidget *createInstructionsScreen();
    QWidget *createCreditsScreen();

    // UI helper method
    QPushButton *createMenuButton(const QString &text);
};

#endif // MAINWINDOW_H