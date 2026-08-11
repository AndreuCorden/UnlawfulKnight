#ifndef WORLD0_H
#define WORLD0_H

#include "src/Worlds/BaseWorld.h"
#include <QTimer>
#include <QSet>
#include <QPixmap>
#include <QVector>
#include <QString>

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
    QPixmap getTilePixmap(char tileType) const;

    QTimer *m_gameLoopTimer;
    QSet<int> m_activeKeys;

    double m_playerX = 5.0;
    double m_playerY = 5.0;
    double m_moveSpeed = 0.08;

    // Wind animation counter for wheat
    int m_windTick = 0;
    const QVector<int> m_wheatAnimSequence = {0, 1, 2, 1};

    // Camera / Viewport Settings (number of visible tiles on screen)
    const double m_visibleCols = 16.0; 
    const double m_visibleRows = 10.0;

    // Map Dimensions
    int m_gridRows = 0;
    int m_gridCols = 0;

    QVector<QString> m_mapGrid = {
        "001200000000000000000000001200",
        "001100000000000000000000001200",
        "002100000000000000000000003300",
        "231223113231233211331232322132",
        "213312233112223323123231123223",
        "001200000000000000000000001300",
        "002300000000000000000000002200",
        "002300000000000000000000001100",
        "003200000000000000000000001200",
        "003100000000000000000000002300",
        "002100000000000000000000002300",
        "001200000000000000000000001200",
        "002300000000000000000000003100",
        "001200000000000000000000003100",
        "003100000000000000000000001200",
        "002300000000000000000000002100",
        "002200000000000000000000001300",
        "001300000000000000000000001300",
        "003100000000000000000000001200",
        "002100000000000000000000001300",
        "001200000000000000000000003300",
        "002300000000000000000000002200",
        "003300000000000000000000003200",
        "003100000000000000000000003100",
        "002100000000000000000000002100",
        "212231123331223123212311232311",
        "311321233231323112322132312322",
        "002200000000000000000000001200",
        "002300000000000000000000002100",
        "001300000000000000000000003100"
    };

    QPixmap m_mapSquares;
};

#endif // WORLD0_H