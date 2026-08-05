#ifndef BASEWORLD_H
#define BASEWORLD_H

#include <QWidget>
#include "src/Player.h"
#include "src/Worlds/WorldId.h"

class BaseWorld : public QWidget {
    Q_OBJECT
public:
    explicit BaseWorld(Player *player, QWidget *parent = nullptr)
        : QWidget(parent), m_player(player) {}
    virtual ~BaseWorld() = default;

    // Sets player starting tile coordinate when entering this map
    virtual void setPlayerPosition(double tileX, double tileY) = 0;

signals:
    // Emitted when stepping into a doorway, edge of map, or map icon
    void requestWorldChange(WorldId targetWorld, double targetX, double targetY);

protected:
    Player *m_player;
};

#endif // BASEWORLD_H