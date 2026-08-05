#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // 1. Frameless Window setup
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    showFullScreen();

    // 2. Global Dark Medieval Styling
    setStyleSheet(R"(
        QMainWindow {
            background-color: #1a1a1a;
            border: 2px solid #4a3b2c;
        }
        QLabel {
            color: #dcdcdc;
            font-family: 'Segoe UI', sans-serif;
        }
        QPushButton {
            background-color: #2b2b2b;
            color: #e0e0e0;
            border: 1px solid #5a4b3c;
            border-radius: 4px;
            padding: 10px 20px;
            font-size: 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #3d3126;
            border: 1px solid #8c6d46;
            color: #ffffff;
        }
        QPushButton:pressed {
            background-color: #1f1812;
        }
    )");

    // 3. Controller & Navigation Setup
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    m_gameController = new GameController(stackedWidget, this);

    // Return to Save Selection when controller requests it
    connect(m_gameController, &GameController::returnToSaveSelectRequested, [this]() {
        stackedWidget->setCurrentIndex(1);
    });

    // Static Pages Setup
    stackedWidget->addWidget(createMainMenuScreen());   // Index 0
    stackedWidget->addWidget(createSaveSelectScreen()); // Index 1
    stackedWidget->addWidget(createInstructionsScreen());// Index 2
    stackedWidget->addWidget(createCreditsScreen());   // Index 3

    stackedWidget->setCurrentIndex(0);
}

QPushButton *MainWindow::createMenuButton(const QString &text)
{
    QPushButton *btn = new QPushButton(text, this);
    btn->setFixedWidth(260);
    btn->setCursor(Qt::PointingHandCursor);
    return btn;
}

// --- MAIN MENU SCREEN ---
QWidget *MainWindow::createMainMenuScreen()
{
    QWidget *screen = new QWidget(this);

    QVBoxLayout *outerLayout = new QVBoxLayout(screen);
    outerLayout->setAlignment(Qt::AlignCenter);

    QWidget *menuBox = new QWidget(screen);
    QVBoxLayout *menuLayout = new QVBoxLayout(menuBox);
    menuLayout->setAlignment(Qt::AlignCenter);
    menuLayout->setSpacing(15);
    menuLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *title = new QLabel("UNLAWFUL KNIGHT", menuBox);
    title->setStyleSheet("font-size: 34px; font-weight: bold; color: #c49a45; margin-bottom: 20px;");
    title->setAlignment(Qt::AlignCenter);

    QPushButton *playBtn = createMenuButton("Play");
    QPushButton *instructionsBtn = createMenuButton("Instructions");
    QPushButton *creditsBtn = createMenuButton("Credits");
    QPushButton *exitBtn = createMenuButton("Exit Game");

    int menuWidth = 480;
    playBtn->setFixedWidth(menuWidth);
    instructionsBtn->setFixedWidth(menuWidth);
    creditsBtn->setFixedWidth(menuWidth);
    exitBtn->setFixedWidth(menuWidth);

    menuLayout->addWidget(title);
    menuLayout->addWidget(playBtn);
    menuLayout->addWidget(instructionsBtn);
    menuLayout->addWidget(creditsBtn);
    menuLayout->addWidget(exitBtn);

    outerLayout->addWidget(menuBox);

    connect(playBtn, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(1); });
    connect(instructionsBtn, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(2); });
    connect(creditsBtn, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(3); });
    connect(exitBtn, &QPushButton::clicked, qApp, &QApplication::quit);

    return screen;
}

// --- SAVE SELECTION SCREEN (PLAY) ---
QWidget *MainWindow::createSaveSelectScreen()
{
    QWidget *screen = new QWidget(this);

    QVBoxLayout *outerLayout = new QVBoxLayout(screen);
    outerLayout->setAlignment(Qt::AlignCenter);

    QWidget *menuBox = new QWidget(screen);
    QVBoxLayout *menuLayout = new QVBoxLayout(menuBox);
    menuLayout->setAlignment(Qt::AlignCenter);
    menuLayout->setSpacing(15);

    QLabel *title = new QLabel("Select Save File", menuBox);
    title->setStyleSheet("font-size: 28px; font-weight: bold; color: #c49a45; margin-bottom: 10px;");
    title->setAlignment(Qt::AlignCenter);

    int menuWidth = 480;
    QPushButton *save1 = createMenuButton("Save Slot 1 (Empty)");
    QPushButton *save2 = createMenuButton("Save Slot 2 (Empty)");
    QPushButton *save3 = createMenuButton("Save Slot 3 (Empty)");
    QPushButton *backBtn = createMenuButton("Back to Menu");

    save1->setFixedWidth(menuWidth);
    save2->setFixedWidth(menuWidth);
    save3->setFixedWidth(menuWidth);
    backBtn->setFixedWidth(menuWidth);

    menuLayout->addWidget(title);
    menuLayout->addWidget(save1);
    menuLayout->addWidget(save2);
    menuLayout->addWidget(save3);
    menuLayout->addSpacing(15);
    menuLayout->addWidget(backBtn);

    outerLayout->addWidget(menuBox);

    connect(save1, &QPushButton::clicked, [this]() { m_gameController->startNewGame(1); });
    connect(save2, &QPushButton::clicked, [this]() { m_gameController->startNewGame(2); });
    connect(save3, &QPushButton::clicked, [this]() { m_gameController->startNewGame(3); });
    connect(backBtn, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(0); });

    return screen;
}

// --- INSTRUCTIONS SCREEN ---
QWidget *MainWindow::createInstructionsScreen()
{
    QWidget *screen = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(screen);
    layout->setContentsMargins(50, 40, 50, 40);

    QLabel *title = new QLabel("Instructions & Tale", screen);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #c49a45;");
    title->setAlignment(Qt::AlignCenter);

    QLabel *content = new QLabel(screen);
    content->setWordWrap(true);
    content->setStyleSheet("font-size: 14px; line-height: 1.4; color: #cccccc;");
    content->setText(
        "<b>THE TALE:</b><br>"
        "Fed up with peasant life, you make a deal with a passing travelling chronicler. "
        "In exchange for forging your certificate of knighthood, the writer accompanies you, "
        "recording your deeds to craft an epic story.<br>"
        "<i>Note: Saving the game represents the chronicler putting pen to paper on recent events.</i><br><br>"

        "<b>OBJECTIVE:</b><br>"
        "Travel from town to town, enter tournaments, purchase better armor & weapons, "
        "and prove your worth in the Grand Tournament.<br><br>"

        "<b>PLAYER STATS:</b><br>"
        "• <b>Strength:</b> Dictates attack damage.<br>"
        "• <b>Stamina:</b> Drains when attacking; determines if you can block with your shield.<br>"
        "• <b>Health & Money:</b> Vital for survival and purchasing gear.<br>"
        "• <b>Reputation:</b> Your fame across the realm.<br><br>"

        "<b>CONTROLS:</b><br>"
        "• <b>WASD / Q-E:</b> Maneuver around your opponent.<br>"
        "• <b>Arrow Keys:</b> Aim strike direction.<br>"
        "• <b>Space Bar:</b> Execute attack.");

    QPushButton *backBtn = createMenuButton("Back");

    layout->addWidget(title);
    layout->addSpacing(15);
    layout->addWidget(content);
    layout->addStretch();
    layout->addWidget(backBtn, 0, Qt::AlignCenter);

    connect(backBtn, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(0); });

    return screen;
}

// --- CREDITS SCREEN ---
QWidget *MainWindow::createCreditsScreen()
{
    QWidget *screen = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(screen);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    QLabel *title = new QLabel("Credits", screen);
    title->setStyleSheet("font-size: 28px; font-weight: bold; color: #c49a45;");

    QLabel *author = new QLabel("Made by: Andreu", screen);
    author->setStyleSheet("font-size: 20px; color: #ffffff;");

    QPushButton *backBtn = createMenuButton("Back");

    layout->addWidget(title);
    layout->addWidget(author);
    layout->addSpacing(30);
    layout->addWidget(backBtn);

    connect(backBtn, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(0); });

    return screen;
}