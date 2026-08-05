// IntroScreenController.h
#ifndef INTROSCREENCONTROLLER_H
#define INTROSCREENCONTROLLER_H

#include <QObject>
#include "IntroScreen.h"

class IntroScreenController : public QObject {
    Q_OBJECT

public:
    explicit IntroScreenController(QObject *parent = nullptr);

    IntroScreen* getView(QWidget *parent = nullptr);
    void startIntroForSlot(int slotId);

signals:
    void backToSaveSelectRequested();
    void gameStarted(int slotId);

private:
    IntroScreen *m_introView = nullptr;
    int m_activeSaveSlot = -1;

    void setupConnections();
};

#endif // INTROSCREENCONTROLLER_H