#include "IntroScreen.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QDebug>
#include <QtMath>

IntroScreen::IntroScreen(QWidget *parent) : QWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);

    loadAssets();
    setupUI();
    locateEntitiesInGrid();

    // 1. Wind Animation Timer
    m_wheatAnimTimer = new QTimer(this);
    connect(m_wheatAnimTimer, &QTimer::timeout, this, [this]() {
        m_windTick++;
        update();
    });
    m_wheatAnimTimer->start(300);

    // 2. Peasant Scythe Animation Timer
    m_plowAnimTimer = new QTimer(this);
    connect(m_plowAnimTimer, &QTimer::timeout, this, [this]() {
        if (m_isPlowing && !m_peasantFrames.isEmpty() && m_currentPeasantFrame < (m_peasantFrames.size() - 1)) {
            m_currentPeasantFrame++;
            if (m_currentPeasantFrame >= (m_peasantFrames.size() - 3)) {
                m_currentPeasantFrame = 0;
                m_isPlowing = false;
                m_plowAnimTimer->stop();
            }
            update();
        }
    });

    m_dropTimer = new QTimer(this);
    connect(m_dropTimer, &QTimer::timeout, this, [this](){
        m_currentPeasantFrame++;
        if (!m_peasantFrames.empty() && m_currentPeasantFrame >= m_peasantFrames.size())
        {
            m_currentPeasantFrame--;
            m_dropTimer->stop();
        }
    });

    // 3. Writer Walk Movement Timer
    m_writerWalkTimer = new QTimer(this);
    connect(m_writerWalkTimer, &QTimer::timeout, this, [this]() {
        double targetCol = m_peasantCol + 1.5; // Stops right near peasant

        if (m_writerColPos < targetCol) {
            m_writerColPos += 0.08; // Step speed
            m_writerSeqIdx = (m_writerSeqIdx + 1) % m_writerSequence.size();
            update();
        } else {
            // Writer arrived
            m_writerWalkTimer->stop();
            m_writerSeqIdx = 0; // Stand still (Writer_NoPose)
            m_currentState = DialoguePhase;
            m_storyBox->show();
            advanceDialogue();
            update();
        }
    });

    // 4. Initial Delay before first prompt
    QTimer::singleShot(2000, this, [this]() {
        if (m_currentState == WaitingForPrompt) {
            m_currentState = PlowingPhase;
            m_promptLabel->setText("[ SPACE ] To Swing Scythe");
            m_promptLabel->show();
        }
    });
}

void IntroScreen::locateEntitiesInGrid() {
    for (int r = 0; r < m_mapGrid.size(); ++r) {
        int c = m_mapGrid[r].indexOf('P');
        if (c != -1) {
            m_peasantRow = r;
            m_peasantCol = c;
            m_writerRow = qMax(0, r - 1); // Place Writer 1 row above Peasant for depth
            break;
        }
    }
}

void IntroScreen::loadAssets() {
    m_wheatFrames.clear();
    m_peasantFrames.clear();
    m_writerFrames.clear();

    // 1. Wheat Frames
    QStringList wheatFiles = {"Wheat_Block.png", "Wheat_Block2.png", "Wheat_Block3.png"};
    for (const QString &file : wheatFiles) {
        QPixmap pix(":/assets/IntroScene/" + file);
        if (pix.isNull()) pix.load("assets/IntroScene/" + file);
        if (!pix.isNull()) m_wheatFrames.append(pix);
    }

    // 2. Peasant Frames
    QStringList peasantSeq = {
        "Person_EndPose.png", "Person_MidPose.png", "Person_Mid2.png",
        "Person_Start.png",   "Person_Mid2.png",   "Person_MidPose.png", "Person_EndPose.png",
        "Person_Drop1", "Person_Drop2", "Person_Drop3"
    };
    for (const QString &file : peasantSeq) {
        QPixmap pix(":/assets/IntroScene/Person/Person1/" + file);
        if (pix.isNull()) pix.load("assets/IntroScene/Person/Person1/" + file);
        if (!pix.isNull()) m_peasantFrames.append(pix);
    }

    // 3. Writer Frames (0: Standing, 1: Walk1, 2: Walk2)
    QStringList writerFiles = {"Writer_NoPose.png", "Writer_Walk1.png", "Writer_Walk2.png"};
    for (const QString &file : writerFiles) {
        QPixmap pix(":/assets/IntroScene/Person/Writer/" + file);
        if (pix.isNull()) pix.load("assets/IntroScene/Person/Writer/" + file);
        if (pix.isNull()) pix.load("assets/IntroScene/Person/" + file);
        if (!pix.isNull()) m_writerFrames.append(pix);
    }
}

void IntroScreen::setupUI() {
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(20, 20, 20, 20);

    // Prompt Banner (Top)
    m_promptLabel = new QLabel(this);
    m_promptLabel->setAlignment(Qt::AlignCenter);
    m_promptLabel->setStyleSheet(R"(
        QLabel {
            background-color: rgba(20, 18, 15, 220);
            color: #f0c050;
            font-size: 17px;
            font-weight: bold;
            border: 2px solid #8c6d46;
            border-radius: 6px;
            padding: 10px 20px;
        }
    )");
    m_promptLabel->hide();

    outerLayout->addWidget(m_promptLabel, 0, Qt::AlignHCenter | Qt::AlignTop);

    m_characterText = new QLabel(this);
    m_characterText->setAlignment(Qt::AlignCenter);
    m_characterText->setStyleSheet(R"(
        QLabel {
            background-color: rgba(20, 18, 15, 220);
            color: #f0c050;
            font-size: 17px;
            font-weight: bold;
            border: 2px solid #8c6d46;
            border-radius: 6px;
            padding: 10px 20px;
        }
    )");
    m_characterText->hide();
    outerLayout->addWidget(m_characterText,0,Qt::AlignHCenter | Qt::AlignTop);
    outerLayout->addStretch();

    // Dialogue Box (Bottom)
    m_storyBox = new QWidget(this);
    m_storyBox->setFixedWidth(650);
    m_storyBox->setStyleSheet(R"(
        QWidget {
            background-color: rgba(20, 18, 15, 235);
            border: 2px solid #5a4b3c;
            border-radius: 8px;
            padding: 15px;
        }
        QPushButton {
            background-color: #3d3126;
            color: #e0e0e0;
            border: 1px solid #8c6d46;
            border-radius: 4px;
            padding: 8px 16px;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #5a4b3c;
            color: #ffffff;
        }
    )");

    QVBoxLayout *storyLayout = new QVBoxLayout(m_storyBox);
    m_dialogueSpeakerLabel = new QLabel(m_storyBox);
    m_dialogueSpeakerLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #c49a45; border: none;");

    m_dialogueTextLabel = new QLabel(m_storyBox);
    m_dialogueTextLabel->setWordWrap(true);
    m_dialogueTextLabel->setStyleSheet("font-size: 15px; color: #dcdcdc; border: none;");

    m_continueBtn = new QPushButton("Next [ SPACE ]", m_storyBox);

    storyLayout->addWidget(m_dialogueSpeakerLabel);
    storyLayout->addWidget(m_dialogueTextLabel);
    storyLayout->addWidget(m_continueBtn, 0, Qt::AlignRight);

    outerLayout->addWidget(m_storyBox, 0, Qt::AlignHCenter | Qt::AlignBottom);
    m_storyBox->hide();

    m_dialogueList = {
        {"Traveling Chronicler", "Greetings, friend. You swing that scythe with the rhythm of a warrior."},
        {"Weary Peasant", "I am no warrior. Just a humble farmer working the scorching fields."},
        {"Traveling Chronicler", "Yet the kingdom bleeds while men of true strength tend to grain."},
        {"Weary Peasant", "Speak clearly, traveler. What calling?"},
        {"Traveling Chronicler", "Drop your plow. Take up the mantle, and let us write your legend!"}
    };

    connect(m_continueBtn, &QPushButton::clicked, this, &IntroScreen::advanceDialogue);
}

void IntroScreen::triggerPlowAction() {
    if (!m_isPlowing && !m_peasantFrames.isEmpty()) {
        m_isPlowing = true;
        m_currentPeasantFrame = 0;
        m_plowAnimTimer->start(125);

        // Display narrative text corresponding to swing count
        m_characterText->setText(m_plowNarrative[m_swingCount%m_plowNarrative.size()]);
        m_characterText->show();

        m_swingCount++;

        // After 3 swings, trigger transition prompt
        if (m_swingCount >= 3 && m_currentState == PlowingPhase) {
            m_currentState = DropPlowPrompt;
            QTimer::singleShot(1200, this, [this]() {
                m_promptLabel->setText("[ DOWN ARROW ] Drop your plow and face the traveler");
            });
        }
    }
}

void IntroScreen::startWriterArrival() {
    m_currentState = WriterWalking;
    m_characterText->hide();
    m_promptLabel->hide();
    m_currentPeasantFrame = m_peasantFrames.size() - 4;
    m_dropTimer->start(125);
    m_writerColPos = -1.5; // Start walking from off-screen left
    m_writerWalkTimer->start(120);
}

void IntroScreen::advanceDialogue() {
    if (m_currentDialogueIndex < m_dialogueList.size()) {
        const auto &line = m_dialogueList[m_currentDialogueIndex];
        m_dialogueSpeakerLabel->setText(line.speaker);
        m_dialogueTextLabel->setText(line.text);
        m_currentDialogueIndex++;
    } else {
        emit gameStartedRequested();
    }
}

void IntroScreen::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {
        if (m_currentState == PlowingPhase || m_currentState == DropPlowPrompt) {
            triggerPlowAction();
        } else if (m_currentState == DialoguePhase) {
            advanceDialogue();
        }
    } else if (event->key() == Qt::Key_Down) {
        if (m_currentState == DropPlowPrompt) {
            startWriterArrival();
        }
    } else {
        QWidget::keyPressEvent(event);
    }
}

// -----------------------------------------------------------------------------
// Y-SORTED DEPTH RENDER LOOP
// -----------------------------------------------------------------------------
void IntroScreen::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, false);

    painter.fillRect(rect(), QColor(25, 18, 12)); // Soil base color

    if (m_wheatFrames.isEmpty() || m_mapGrid.isEmpty()) return;

    int gridRows = m_mapGrid.size();
    int gridCols = m_mapGrid[0].length();

    int baseW = m_wheatFrames[0].width();
    int baseH = m_wheatFrames[0].height();

    double overlapRatio = 0.15;
    double unscaledGridW = gridCols * baseW;
    double unscaledGridH = (gridRows * baseH * (1.0 - overlapRatio)) + (baseH * overlapRatio);

    double scale = qMin(static_cast<double>(width()) / unscaledGridW,
                        static_cast<double>(height()) / unscaledGridH);

    int tileW = static_cast<int>(baseW * scale);
    int tileH = static_cast<int>(baseH * scale);
    int overlapY = static_cast<int>(tileH * overlapRatio);
    int strideY = tileH - overlapY;

    int totalWidth = gridCols * tileW;
    int totalHeight = (gridRows * strideY) + overlapY;

    int startX = (width() - totalWidth) / 2;
    int startY = (height() - totalHeight) / 2;

    const QPixmap &currentPeasant = m_peasantFrames.isEmpty() ? QPixmap() : m_peasantFrames[m_currentPeasantFrame];

    QPixmap currentWriter;
    if (!m_writerFrames.isEmpty()) {
        int idx = m_writerSequence[m_writerSeqIdx];
        if (idx < m_writerFrames.size()) currentWriter = m_writerFrames[idx];
    }

    // --- ROW-BY-ROW Y-SORT PASS ---
    for (int row = 0; row < gridRows; ++row) {

        // 1. Draw Wheat Tiles for current row
        for (int col = 0; col < gridCols; ++col) {
            int waveOffset = col + (row / 2);
            int phase = (m_windTick + waveOffset) % m_wheatSequence.size();
            int frameIdx = m_wheatSequence[phase];

            int drawX = startX + (col * tileW);
            int drawY = startY + (row * strideY) - overlapY;

            painter.drawPixmap(drawX, drawY, tileW, tileH, m_wheatFrames[frameIdx]);
        }

        // 2. Render Writer on Writer's Row
        if (m_currentState >= WriterWalking && row == m_writerRow && !currentWriter.isNull()) {
            int wW = static_cast<int>(currentWriter.width() * scale);
            int wH = static_cast<int>(currentWriter.height() * scale);

            int writerX = startX + static_cast<int>(m_writerColPos * tileW) + (tileW - wW) / 2;
            int writerY = startY + (row * strideY) - overlapY + (tileH - wH);

            painter.drawPixmap(writerX, writerY, wW, wH, currentWriter);
        }

        // 3. Render Peasant on Peasant's Row
        if (row == m_peasantRow && !currentPeasant.isNull()) {
            int pW = static_cast<int>(currentPeasant.width() * scale);
            int pH = static_cast<int>(currentPeasant.height() * scale);

            int playerX = startX + (m_peasantCol * tileW) + (tileW - pW) / 2;
            int playerY = startY + (row * strideY) - overlapY + (tileH - pH);

            painter.drawPixmap(playerX, playerY, pW, pH, currentPeasant);
        }
    }
}