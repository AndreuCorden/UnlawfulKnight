#include "IntroScreen.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

IntroScreen::IntroScreen(QWidget *parent) : QWidget(parent) {
    loadAssets();
    setupUI();

    // 150ms delay creates a smooth pixel-art animation loop
    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, [this]() {
        if (!wheatFrames.isEmpty()) {
            currentFrameIndex = (currentFrameIndex + 1) % wheatFrames.size();
            update(); // Trigger repaint with the next frame
        }
    });
    animTimer->start(150);
}

void IntroScreen::loadAssets() {
    wheatFrames.clear();

    // List of frame file names in your assets folder
    QStringList frameNames = {
        "Wheat-Block.png",
        "Wheat_Bloc2.png",
        "Wheat_Bloc3.png"
    };

    for (const QString &name : frameNames) {
        QPixmap pix;

        // 1. Try QRC resource path
        pix.load(":/assets/" + name);

        // 2. Fallbacks if QRC path differs or running loose build
        if (pix.isNull()) {
            // Check alternate spelling for frame 1 if needed
            if (name == "Wheat-Block.png") {
                pix.load(":/assets/Wheat_Block.png");
            }
        }
        if (pix.isNull()) pix.load("assets/" + name);
        if (pix.isNull()) pix.load(name);

        if (!pix.isNull()) {
            wheatFrames.append(pix);
            qDebug() << "[SUCCESS] Loaded frame:" << name;
        } else {
            qWarning() << "[WARNING] Could not load animation frame:" << name;
        }
    }
}

void IntroScreen::setupUI() {
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setAlignment(Qt::AlignCenter);

    // Story box overlay
    QWidget *storyBox = new QWidget(this);
    storyBox->setFixedWidth(650);
    storyBox->setStyleSheet(R"(
        QWidget {
            background-color: rgba(20, 18, 15, 230);
            border: 2px solid #5a4b3c;
            border-radius: 8px;
            padding: 25px;
        }
        QLabel {
            color: #dcdcdc;
            font-size: 16px;
            line-height: 1.5;
        }
        QPushButton {
            background-color: #3d3126;
            color: #e0e0e0;
            border: 1px solid #8c6d46;
            border-radius: 4px;
            padding: 10px 20px;
            font-size: 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #5a4b3c;
            color: #ffffff;
        }
    )");

    QVBoxLayout *storyLayout = new QVBoxLayout(storyBox);
    storyLayout->setSpacing(15);

    QLabel *title = new QLabel("The Peasant's Tale Begins", storyBox);
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #c49a45; border: none; padding: 0;");
    title->setAlignment(Qt::AlignCenter);

    QLabel *storyText = new QLabel(
        "You stand in the scorching sun, weary from another day in the fields. "
        "A travelling chronicler approaches, resting along the dusty road.<br><br>"
        "<i>\"A man of your vigor wasted on wheat?\"</i> he muses. "
        "<i>\"I can forge you a certificate of knighthood... if you allow me to record your journey into legend.\"</i>",
        storyBox
    );
    storyText->setWordWrap(true);
    storyText->setStyleSheet("border: none; padding: 0;");

    QPushButton *continueBtn = new QPushButton("Accept Offer & Begin", storyBox);
    QPushButton *backBtn = new QPushButton("Back to Save Select", storyBox);

    storyLayout->addWidget(title);
    storyLayout->addWidget(storyText);
    storyLayout->addSpacing(15);
    storyLayout->addWidget(continueBtn);
    storyLayout->addWidget(backBtn);

    outerLayout->addWidget(storyBox);

    connect(backBtn, &QPushButton::clicked, this, &IntroScreen::backToMenuRequested);
}

void IntroScreen::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);

    // Soil background behind wheat tiles
    painter.fillRect(rect(), QColor(25, 18, 12));

    if (wheatFrames.isEmpty()) return;

    // Crisp pixel art rendering
    painter.setRenderHint(QPainter::SmoothPixmapTransform, false);

    const QPixmap &currentTile = wheatFrames[currentFrameIndex];

    // Native scale (1 = 100% size). Change to 1.5 or 2 if you want it slightly chunkier
    double scale = 0.5;
    int tileW = static_cast<int>(currentTile.width() * scale);
    int tileH = static_cast<int>(currentTile.height() * scale);

    if (tileW <= 0 || tileH <= 0) return;

    // Tile across the screen area
    for (int y = 0; y < height(); y += tileH) {
        for (int x = 0; x < width(); x += tileW) {
            painter.drawPixmap(x, y, tileW, tileH, currentTile);
        }
    }
}