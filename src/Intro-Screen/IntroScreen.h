#ifndef INTROSCREEN_H
#define INTROSCREEN_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QVector>
#include <QStringList>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>

struct DialogueLine {
    QString speaker;
    QString text;
};

class IntroScreen : public QWidget {
    Q_OBJECT

public:
    explicit IntroScreen(QWidget *parent = nullptr);

signals:
    void backToMenuRequested();
    void gameStartedRequested();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    // --- 1. SCENE STATE MACHINE ---
    enum State {
        WaitingForPrompt,   // Initial silence/delay
        PlowingPhase,       // Player swinging scythe
        DropPlowPrompt,     // Prompt to press Down Arrow
        WriterWalking,      // Writer entering screen
        DialoguePhase       // Active dialogue box
    };

    State m_currentState = WaitingForPrompt;

    // --- 2. ASSETS & ANIMATION SEQUENCES ---
    QVector<QPixmap> m_wheatFrames;
    QVector<QPixmap> m_writerFrames;   // 0: Stand/NoPose, 1: Walk1, 2: Walk2
    QVector<QPixmap> m_peasantFrames;

    QVector<int> m_wheatSequence = {0, 1, 2, 1};
    QVector<int> m_writerSequence = {0, 1, 0, 2}; // 0 -> 1 -> 0 -> 2 walk loop

    // --- 3. MAP GRID & ENTITY POSITIONS ---
    QVector<QString> m_mapGrid = {
        "wwwwwwwww",
        "wwwwwwwww",
        "wwwwPwwww", // Row 2, Col 4
        "wwwwwwwww",
        "wwwwwwwww"
    };

    int m_peasantRow = 2;
    int m_peasantCol = 4;

    int m_writerRow = 1;          // Writer stands one row above Peasant for depth
    double m_writerColPos = -2.0; // Starts off-screen left
    int m_writerSeqIdx = 0;

    // --- 4. GAMEPLAY & NARRATIVE COUNTERS ---
    int m_windTick = 0;
    int m_swingCount = 0;
    int m_currentPeasantFrame = 0;
    bool m_isPlowing = false;

    // Story text shown after each scythe swing
    const QVector<QString> m_plowNarrative = {
        "Hm...",
        "I can't go on like this.",
        "I have to do something else with my life."
    };

    // --- 5. DIALOGUE SYSTEM ---
    QVector<DialogueLine> m_dialogueList;
    int m_currentDialogueIndex = 0;

    // --- 6. TIMERS ---
    QTimer *m_wheatAnimTimer = nullptr;
    QTimer *m_plowAnimTimer = nullptr;
    QTimer *m_writerWalkTimer = nullptr;

    // --- 7. UI COMPONENTS ---
    QLabel *m_promptLabel = nullptr;
    QLabel *m_characterText = nullptr;
    QWidget *m_storyBox = nullptr;
    QLabel *m_dialogueSpeakerLabel = nullptr;
    QLabel *m_dialogueTextLabel = nullptr;
    QPushButton *m_continueBtn = nullptr;

    // --- HELPER METHODS ---
    void loadAssets();
    void setupUI();
    void triggerPlowAction();
    void startWriterArrival();
    void advanceDialogue();
    void locateEntitiesInGrid();
};

#endif // INTROSCREEN_H