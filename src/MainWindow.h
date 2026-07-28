#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "src/Intro-Screen/IntroScreenController.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stackedWidget;
    IntroScreenController *m_introController;

    // Screen setup methods
    QWidget *createMainMenuScreen();
    QWidget *createSaveSelectScreen();
    QWidget *createInstructionsScreen();
    QWidget *createCreditsScreen();

    // Helper to request intro launch from controller
    void launchIntroScreen(int slotId);

    // UI helper method
    QPushButton *createMenuButton(const QString &text);
};

#endif // MAINWINDOW_H