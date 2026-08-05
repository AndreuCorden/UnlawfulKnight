#ifndef WORLD0_H
#define WORLD0_H

#include "src/Worlds/BaseWorld.h"
#include <QTimer>
#include <QSet>

class World0 : public BaseWorld {
    Q_OBJECT
public:
    explicit World0(Player *player, QWidget *parent = nullptr);
    void setPlayerPosition(double tileX, double tileY) override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void updateMovement();

    QTimer *m_gameLoopTimer;
    QSet<int> m_activeKeys;

    double m_playerX = 5.0;
    double m_playerY = 5.0;
    double m_moveSpeed = 0.08;

    int m_gridRows = 15;
    int m_gridCols = 20;
};

#endif // WORLD0_H