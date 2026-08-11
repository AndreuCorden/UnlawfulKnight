#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QStackedWidget>
#include "src/Player.h"
#include "src/Worlds/WorldId.h"
#include "src/Worlds/BaseWorld.h"
#include "src/Intro-Screen/NameEntryWidget.h"
#include "src/Intro-Screen/IntroScreenController.h"

class GameController : public QObject {
    Q_OBJECT
public:
    explicit GameController(QStackedWidget *stackedWidget, QObject *parent = nullptr);
    ~GameController();

    void startNewGame(int saveSlotId);
    void loadWorld(WorldId worldId, double startX, double startY);

signals:
    void returnToSaveSelectRequested();

private:
    void showNameEntry();
    BaseWorld* createWorldInstance(WorldId worldId);

    QStackedWidget *m_stackedWidget;
    int m_saveSlotId = -1;

    Player *m_player = nullptr;
    IntroScreenController *m_introController = nullptr;
    NameEntryWidget *m_nameEntryWidget = nullptr;
    BaseWorld *m_currentWorld = nullptr;
};

#endif // GAMECONTROLLER_H