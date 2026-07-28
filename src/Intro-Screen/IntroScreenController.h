#ifndef INTROSCREENCONTROLLER_H
#define INTROSCREENCONTROLLER_H

#include <QObject>
#include "src/Intro-Screen/IntroScreen.h"

class IntroScreenController : public QObject {
    Q_OBJECT

public:
    explicit IntroScreenController(QObject *parent = nullptr);
    ~IntroScreenController() = default;

    // Instantiates or retrieves the managed IntroScreen view
    IntroScreen* getView(QWidget *parent = nullptr);

    // Called when a save slot is selected
    void startIntroForSlot(int slotId);

signals:
    // Emitted back to MainWindow when the player clicks "Back"
    void backToSaveSelectRequested();

    // Emitted when the player accepts the offer and begins game
    void gameStarted(int slotId);

private:
    IntroScreen *m_introView = nullptr;
    int m_activeSaveSlot = -1;

    void setupConnections();
};

#endif // INTROSCREENCONTROLLER_H