#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stackedWidget;

    // Screen setup methods
    QWidget *createMainMenuScreen();
    QWidget *createSaveSelectScreen();
    QWidget *createInstructionsScreen();
    QWidget *createCreditsScreen();

    // UI helper method
    QPushButton *createMenuButton(const QString &text);
};

#endif // MAINWINDOW_H