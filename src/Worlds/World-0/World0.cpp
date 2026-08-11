#include "World0.h"
#include <QPainter>
#include <QKeyEvent>
#include <QtMath>
#include <algorithm>

World0::World0(Player *player, QWidget *parent)
    : BaseWorld(player, parent) 
{
    setFocusPolicy(Qt::StrongFocus);

    m_gridRows = m_mapGrid.size();
    m_gridCols = m_mapGrid.isEmpty() ? 0 : m_mapGrid[0].length();

    // Load the spritesheet (Ensure path matches your Qt resource file or assets folder)
    if (!m_mapSquares.load(":assets/Worlds/World0/World-0Dirtpath.png")) {
        // Error, failed to load.
    }

    m_gameLoopTimer = new QTimer(this);
    connect(m_gameLoopTimer, &QTimer::timeout, this, [this]() {
        updateMovement();
        
        // Wind tick increments every frame for wheat sway animation
        m_windTick++;
        
        update();
    });
    m_gameLoopTimer->start(16);
}

void World0::setPlayerPosition(double tileX, double tileY) {
    m_playerX = tileX;
    m_playerY = tileY;
}

void World0::keyPressEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()) {
        m_activeKeys.insert(event->key());
    }
}

void World0::keyReleaseEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()) {
        m_activeKeys.remove(event->key());
    }
}

void World0::updateMovement() {
    double dx = 0;
    double dy = 0;

    if (m_activeKeys.contains(Qt::Key_W) || m_activeKeys.contains(Qt::Key_Up))    dy -= m_moveSpeed;
    if (m_activeKeys.contains(Qt::Key_S) || m_activeKeys.contains(Qt::Key_Down))  dy += m_moveSpeed;
    if (m_activeKeys.contains(Qt::Key_A) || m_activeKeys.contains(Qt::Key_Left))  dx -= m_moveSpeed;
    if (m_activeKeys.contains(Qt::Key_D) || m_activeKeys.contains(Qt::Key_Right)) dx += m_moveSpeed;

    if (dx != 0 && dy != 0) {
        dx *= 0.7071;
        dy *= 0.7071;
    }

    double nextX = m_playerX + dx;
    double nextY = m_playerY + dy;

    // Transition when exiting map bounds
    if (nextX >= m_gridCols - 0.5) {
        emit requestWorldChange(WorldId::GlobalMap, 2.0, 5.0);
        return;
    }

    m_playerX = qBound(0.0, nextX, m_gridCols - 1.0);
    m_playerY = qBound(0.0, nextY, m_gridRows - 1.0);
}

QPixmap World0::getTilePixmap(char tileType) const {
    if (m_mapSquares.isNull()) {
        return QPixmap();
    }

    int srcX = 0;
    int srcY = 0;

    if (tileType == '0') {
        // Row 0: Animated Wheat
        int animFrameIndex = (m_windTick / 18) % m_wheatAnimSequence.size();
        int frameCol = m_wheatAnimSequence[animFrameIndex];
        srcX = frameCol * 8;
        srcY = 0;
    } else {
        // Row 1: Dirt Variants
        srcY = 8;
        if (tileType == '1')      srcX = 0;   // Dirt 1 -> (1, 0)
        else if (tileType == '2') srcX = 8;   // Dirt 2 -> (1, 1)
        else if (tileType == '3') srcX = 16;  // Dirt 3 -> (1, 2)
    }

    return m_mapSquares.copy(srcX, srcY, 8, 8);
}

void World0::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    // 1. Calculate Tile Scale based on current screen dimension & target visible tiles
    double tileW = static_cast<double>(width()) / m_visibleCols;
    double tileH = static_cast<double>(height()) / m_visibleRows;

    // 2. Camera Offset Calculation (Center player, clamp to map boundaries)
    double targetCamX = m_playerX - (m_visibleCols / 2.0) + 0.5;
    double targetCamY = m_playerY - (m_visibleRows / 2.0) + 0.5;

    double cameraX = qBound(0.0, targetCamX, qMax(0.0, m_gridCols - m_visibleCols));
    double cameraY = qBound(0.0, targetCamY, qMax(0.0, m_gridRows - m_visibleRows));

    // Visible index bounds for optimized drawing
    int startCol = static_cast<int>(std::floor(cameraX));
    int endCol = static_cast<int>(std::ceil(cameraX + m_visibleCols));
    int startRow = static_cast<int>(std::floor(cameraY));
    int endRow = static_cast<int>(std::ceil(cameraY + m_visibleRows));

    startCol = qBound(0, startCol, m_gridCols);
    endCol   = qBound(0, endCol, m_gridCols);
    startRow = qBound(0, startRow, m_gridRows);
    endRow   = qBound(0, endRow, m_gridRows);

    // 3. Render Map Tiles
    for (int r = startRow; r < endRow; ++r) {
        for (int c = startCol; c < endCol; ++c) {
            char tileType = m_mapGrid[r][c].toLatin1();
            
            double drawX = (c - cameraX) * tileW;
            double drawY = (r - cameraY) * tileH;
            QRectF drawRect(drawX, drawY, tileW + 0.5, tileH + 0.5); // +0.5 prevents subpixel gaps

            QPixmap tileImg = getTilePixmap(tileType);
            if (!tileImg.isNull()) {
                painter.drawPixmap(drawRect, tileImg, tileImg.rect());
            } else {
                // Fallback coloring if image is not loaded
                QColor fallback = (tileType == '0') ? QColor(220, 200, 50) : QColor(100, 60, 30);
                painter.fillRect(drawRect, fallback);
            }
        }
    }

    // 4. Render Player (Relative to Camera)
    double playerDrawX = (m_playerX - cameraX) * tileW;
    double playerDrawY = (m_playerY - cameraY) * tileH;

    // Drop Shadow
    painter.setBrush(QColor(0, 0, 0, 100));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QRectF(playerDrawX + tileW * 0.1, playerDrawY + tileH * 0.75, tileW * 0.8, tileH * 0.25));

    // Player Sprite Body
    painter.setBrush(QColor(180, 50, 50));
    painter.setPen(QPen(QColor(20, 20, 20), 2));
    painter.drawRect(QRectF(playerDrawX + tileW * 0.25, playerDrawY + tileH * 0.2, tileW * 0.5, tileH * 0.65));

    // 5. Fixed Screen HUD
    if (m_player) {
        painter.setBrush(QColor(20, 18, 15, 220));
        painter.setPen(QPen(QColor(140, 109, 70), 2));
        painter.drawRoundedRect(15, 15, 220, 110, 6, 6);

        painter.setPen(QColor(240, 192, 80));
        QFont font = painter.font();
        font.setBold(true);
        font.setPixelSize(14);
        painter.setFont(font);

        painter.drawText(25, 35, m_player->name());

        painter.setPen(QColor(220, 220, 220));
        font.setBold(false);
        font.setPixelSize(12);
        painter.setFont(font);

        painter.drawText(25, 55, QString("HP: %1/%2").arg(m_player->currentHealth()).arg(m_player->maxHealth()));
        painter.drawText(25, 70, QString("Stamina: %1/%2").arg(m_player->currentStamina()).arg(m_player->maxStamina()));
        painter.drawText(25, 85, QString("Gold: %1g | Rep: %2").arg(m_player->money()).arg(m_player->reputation()));
        painter.drawText(25, 105, QString("STR: %1").arg(m_player->strength()));
    }
}