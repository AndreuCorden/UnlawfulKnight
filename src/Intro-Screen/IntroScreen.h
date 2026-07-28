#ifndef INTROSCREEN_H
#define INTROSCREEN_H

#include <QWidget>
#include <QPixmap>
#include <QVector>
#include <QTimer>
#include <QKeyEvent>
#include <QLabel>

class IntroScreen : public QWidget {
    Q_OBJECT

public:
    explicit IntroScreen(QWidget *parent = nullptr);

signals:
    void backToMenuRequested();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    // Wheat Sway Animation
    QVector<QPixmap> m_wheatFrames;
    QVector<int> m_wheatSequence = {0, 1, 2, 1};
    int m_windTick = 0;
    QTimer *m_wheatAnimTimer;

    // Peasant Plowing Animation
    QVector<QPixmap> m_peasantFrames;
    int m_currentPeasantFrame = 0;
    bool m_isPlowing = false;
    QTimer *m_plowAnimTimer;

    QVector<QString> m_mapGrid = {
    "wwwwwwwww",
    "wwwwwwwww",
    "wwwwPwwww",
    "wwwwwwwww",
    "wwwwwwwww"
};

    // Floating UI prompt
    QLabel *m_promptLabel;

    void loadAssets();
    void setupUI();
    void triggerPlowAction();
};

#endif // INTROSCREEN_H