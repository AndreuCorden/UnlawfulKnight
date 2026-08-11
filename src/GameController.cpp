#include "GameController.h"
#include "Worlds/World-0/World0.h"

GameController::GameController(QStackedWidget *stackedWidget, QObject *parent)
    : QObject(parent)
    , m_stackedWidget(stackedWidget)
{
    m_introController = new IntroScreenController(this);

    connect(m_introController, &IntroScreenController::backToSaveSelectRequested, this, [this]() {
        emit returnToSaveSelectRequested();
    });

    // Intro complete -> Open name entry widget
    connect(m_introController, &IntroScreenController::gameStarted, this, [this](int slotId) {
        Q_UNUSED(slotId);
        showNameEntry();
    });
}

GameController::~GameController() {
    delete m_player;
}

void GameController::startNewGame(int saveSlotId) {
    m_saveSlotId = saveSlotId;

    m_introController->startIntroForSlot(m_saveSlotId);
    IntroScreen *introView = m_introController->getView(m_stackedWidget);

    if (m_stackedWidget->indexOf(introView) == -1) {
        m_stackedWidget->addWidget(introView);
    }

    m_stackedWidget->setCurrentWidget(introView);
    introView->setFocus();
}

void GameController::showNameEntry() {
    if (!m_nameEntryWidget) {
        m_nameEntryWidget = new NameEntryWidget(m_stackedWidget);
        m_stackedWidget->addWidget(m_nameEntryWidget);

        connect(m_nameEntryWidget, &NameEntryWidget::nameConfirmed, this, [this](const QString &characterName) {
            delete m_player;
            m_player = new Player(characterName);

            // Entry point into the game world array: World0
            loadWorld(WorldId::World0, 2, 27);
        });
    }

    m_stackedWidget->setCurrentWidget(m_nameEntryWidget);
}

void GameController::loadWorld(WorldId worldId, double startX, double startY) {
    // Unload existing world if active
    if (m_currentWorld) {
        m_stackedWidget->removeWidget(m_currentWorld);
        delete m_currentWorld;
        m_currentWorld = nullptr;
    }

    // Instantiate target world based on Enum
    m_currentWorld = createWorldInstance(worldId);

    if (m_currentWorld) {
        m_currentWorld->setPlayerPosition(startX, startY);

        // Connect scene transition requests inside map to GameController
        connect(m_currentWorld, &BaseWorld::requestWorldChange, this, &GameController::loadWorld);

        m_stackedWidget->addWidget(m_currentWorld);
        m_stackedWidget->setCurrentWidget(m_currentWorld);
        m_currentWorld->setFocus();
    }
}

BaseWorld* GameController::createWorldInstance(WorldId worldId) {
    switch (worldId) {
        case WorldId::World0:
            return new World0(m_player, m_stackedWidget);

        // Future map implementations will map here:
        // case WorldId::World1:
        //     return new World1(m_player, m_stackedWidget);
        // case WorldId::GlobalMap:
        //     return new GlobalMap(m_player, m_stackedWidget);

        default:
            return new World0(m_player, m_stackedWidget);
    }
}