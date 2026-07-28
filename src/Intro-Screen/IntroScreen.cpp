#include "IntroScreen.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

IntroScreen::IntroScreen(QWidget *parent) : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);

    loadAssets();
    setupUI();

    // 1. Wind Wave Timer (Advances wind motion across field)
    m_wheatAnimTimer = new QTimer(this);
    connect(m_wheatAnimTimer, &QTimer::timeout, this, [this]()
            {
                m_windTick++;
                update(); // Re-render grid with shifted wind phase
            });
    m_wheatAnimTimer->start(300); // Wind wave speed in milliseconds

    // 2. Peasant Scythe Swing Timer
    m_plowAnimTimer = new QTimer(this);
    connect(m_plowAnimTimer, &QTimer::timeout, this, [this]()
            {
        if (m_isPlowing && !m_peasantFrames.isEmpty()) {
            m_currentPeasantFrame++;
            if (m_currentPeasantFrame >= m_peasantFrames.size()) {
                m_currentPeasantFrame = 0;
                m_isPlowing = false;
                m_plowAnimTimer->stop();
            }
            update();
        } });
}

void IntroScreen::loadAssets()
{
    m_wheatFrames.clear();
    m_peasantFrames.clear();

    // --- 1. Load Wheat Frames ---
    QStringList wheatFiles = {
        "Wheat_Block.png",
        "Wheat_Block2.png",
        "Wheat_Block3.png"};

    for (const QString &file : wheatFiles)
    {
        QPixmap pix(":/assets/IntroScene/" + file);
        if (pix.isNull())
            pix.load("assets/IntroScene/" + file);

        if (!pix.isNull())
        {
            m_wheatFrames.append(pix);
        }
        else
        {
            qWarning() << "[ERROR] Could not load wheat asset:" << file;
        }
    }

    // --- 2. Load Peasant Frames in Exact Sequence: ---
    // Sequence: End -> Mid -> Mid2 -> Start -> Mid2 -> Mid -> End
    QStringList peasantSequence = {
        "Person_EndPose.png",
        "Person_MidPose.png",
        "Person_Mid2.png",
        "Person_Start.png",
        "Person_Mid2.png",
        "Person_MidPose.png",
        "Person_EndPose.png"};

    for (const QString &file : peasantSequence)
    {
        QPixmap pix(":/assets/IntroScene/Person/" + file);
        if (pix.isNull())
            pix.load("assets/IntroScene/Person/" + file);

        if (!pix.isNull())
        {
            m_peasantFrames.append(pix);
        }
        else
        {
            qWarning() << "[ERROR] Could not load peasant frame:" << file;
        }
    }
}

void IntroScreen::setupUI()
{
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(20, 20, 20, 20);

    m_promptLabel = new QLabel("[ SPACE ] To Swing Scythe", this);
    m_promptLabel->setAlignment(Qt::AlignCenter);
    m_promptLabel->setStyleSheet(R"(
        QLabel {
            background-color: rgba(20, 18, 15, 210);
            color: #f0c050;
            font-size: 18px;
            font-weight: bold;
            border: 2px solid #8c6d46;
            border-radius: 6px;
            padding: 10px 20px;
        }
    )");

    outerLayout->addWidget(m_promptLabel, 0, Qt::AlignHCenter | Qt::AlignTop);
    outerLayout->addStretch();

    QWidget *storyBox = new QWidget(this);
    storyBox->setFixedWidth(600);
    storyBox->setStyleSheet(R"(
        QWidget {
            background-color: rgba(20, 18, 15, 225);
            border: 2px solid #5a4b3c;
            border-radius: 8px;
            padding: 20px;
        }
        QLabel {
            color: #dcdcdc;
            font-size: 15px;
        }
        QPushButton {
            background-color: #3d3126;
            color: #e0e0e0;
            border: 1px solid #8c6d46;
            border-radius: 4px;
            padding: 8px 16px;
            font-size: 15px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #5a4b3c;
            color: #ffffff;
        }
    )");

    QVBoxLayout *storyLayout = new QVBoxLayout(storyBox);
    storyLayout->setSpacing(12);

    QLabel *title = new QLabel("The Peasant's Tale Begins", storyBox);
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #c49a45; border: none;");
    title->setAlignment(Qt::AlignCenter);

    QLabel *storyText = new QLabel(
        "You stand in the scorching sun, weary from another day in the fields. "
        "A travelling chronicler approaches along the dusty road...",
        storyBox);
    storyText->setWordWrap(true);
    storyText->setStyleSheet("border: none;");

    QPushButton *continueBtn = new QPushButton("Accept Offer & Begin", storyBox);
    QPushButton *backBtn = new QPushButton("Back to Save Select", storyBox);

    storyLayout->addWidget(title);
    storyLayout->addWidget(storyText);
    storyLayout->addWidget(continueBtn);
    storyLayout->addWidget(backBtn);

    outerLayout->addWidget(storyBox, 0, Qt::AlignHCenter | Qt::AlignBottom);

    connect(backBtn, &QPushButton::clicked, this, &IntroScreen::backToMenuRequested);
}

void IntroScreen::triggerPlowAction()
{
    if (!m_isPlowing && !m_peasantFrames.isEmpty())
    {
        m_isPlowing = true;
        m_currentPeasantFrame = 0;
        m_plowAnimTimer->start(125);
    }
}

void IntroScreen::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space)
    {
        triggerPlowAction();
    }
    else
    {
        QWidget::keyPressEvent(event);
    }
}

void IntroScreen::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, false);

    // Dark soil background coat
    painter.fillRect(rect(), QColor(25, 18, 12));

    if (m_wheatFrames.isEmpty() || m_mapGrid.isEmpty()) return;

    int gridRows = m_mapGrid.size();
    int gridCols = m_mapGrid[0].length();

    // Base dimensions from raw image asset
    int baseW = m_wheatFrames[0].width();
    int baseH = m_wheatFrames[0].height();

    // Overlap percentage (15% of sprite height overlaps with the row above it)
    double overlapRatio = 0.15;

    // Calculate effective unscaled grid dimensions with overlap included
    double unscaledGridW = gridCols * baseW;
    double unscaledGridH = (gridRows * baseH * (1.0 - overlapRatio)) + (baseH * overlapRatio);

    // Calculate dynamic scale factor to fit window screen
    double scaleX = static_cast<double>(width()) / unscaledGridW;
    double scaleY = static_cast<double>(height()) / unscaledGridH;
    double scale = qMin(scaleX, scaleY); // Preserve aspect ratio

    // Scaled tile dimensions
    int tileW = static_cast<int>(baseW * scale);
    int tileH = static_cast<int>(baseH * scale);

    // Stride is the vertical step distance between rows
    int overlapY = static_cast<int>(tileH * overlapRatio);
    int strideY = tileH - overlapY;

    // Centering calculations
    int totalWidth = gridCols * tileW;
    int totalHeight = (gridRows * strideY) + overlapY;

    int startX = (width() - totalWidth) / 2;
    int startY = (height() - totalHeight) / 2;

    const QPixmap &currentPeasant = m_peasantFrames.isEmpty() ? QPixmap() : m_peasantFrames[m_currentPeasantFrame];

    // -------------------------------------------------------------
    // RENDER LOOP (Top-to-Bottom for natural overlap & depth)
    // -------------------------------------------------------------
    for (int row = 0; row < gridRows; ++row) {
        for (int col = 0; col < gridCols; ++col) {

            // --- WIND DIRECTION FORMULA ---
            // (col + row): Wind sweeps diagonally from top-left to bottom-right
            // Change to (col) for left-to-right wind, or (row) for top-to-bottom wind
            int waveOffset = col + (row / 2);
            int phase = (m_windTick + waveOffset) % m_wheatSequence.size();
            int frameIdx = m_wheatSequence[phase];

            const QPixmap &currentWheat = m_wheatFrames[frameIdx];

            int drawX = startX + (col * tileW);
            int drawY = startY + (row * strideY) - overlapY;

            QChar tileType = m_mapGrid[row][col];

            if (tileType == 'w') {
                painter.drawPixmap(drawX, drawY, tileW, tileH, currentWheat);
            } 
            else if (tileType == 'P') {
                // Ground tile underneath peasant
                painter.drawPixmap(drawX, drawY, tileW, tileH, currentWheat);

                // Draw Peasant
                if (!currentPeasant.isNull()) {
                    int pW = static_cast<int>(currentPeasant.width() * scale);
                    int pH = static_cast<int>(currentPeasant.height() * scale);

                    int playerX = drawX + (tileW - pW) / 2;
                    int playerY = drawY + (tileH - pH);

                    painter.drawPixmap(playerX, playerY, pW, pH, currentPeasant);
                }
            }
        }
    }
}