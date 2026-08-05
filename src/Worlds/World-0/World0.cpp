#include "World0.h"
#include <QPainter>
#include <QKeyEvent>

World0::World0(Player *player, QWidget *parent)
    : BaseWorld(player, parent) 
{
    setFocusPolicy(Qt::StrongFocus);

    m_gameLoopTimer = new QTimer(this);
    connect(m_gameLoopTimer, &QTimer::timeout, this, [this]() {
        updateMovement();
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

    // Example Transition: Walking off the right edge loads the Global Map
    if (nextX >= m_gridCols - 0.5) {
        emit requestWorldChange(WorldId::GlobalMap, 2.0, 5.0);
        return;
    }

    m_playerX = qBound(0.0, nextX, m_gridCols - 1.0);
    m_playerY = qBound(0.0, nextY, m_gridRows - 1.0);
}

void World0::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    int tileW = width() / m_gridCols;
    int tileH = height() / m_gridRows;

    // 1. Tile Grid
    for (int r = 0; r < m_gridRows; ++r) {
        for (int c = 0; c < m_gridCols; ++c) {
            QColor tileColor = ((r + c) % 2 == 0) ? QColor(45, 80, 35) : QColor(40, 75, 30);
            painter.fillRect(c * tileW, r * tileH, tileW, tileH, tileColor);
        }
    }

    // 2. Player Character
    int px = static_cast<int>(m_playerX * tileW);
    int py = static_cast<int>(m_playerY * tileH);
    
    painter.setBrush(QColor(0, 0, 0, 100));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(px + 4, py + tileH - 8, tileW - 8, 8);

    painter.setBrush(QColor(180, 50, 50));
    painter.setPen(QPen(QColor(20, 20, 20), 2));
    painter.drawRect(px + tileW / 4, py + tileH / 4, tileW / 2, tileH / 2);

    // 3. Player Stats HUD
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