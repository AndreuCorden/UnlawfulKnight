#ifndef INTROSCREEN_H
#define INTROSCREEN_H

#include <QWidget>
#include <QPixmap>
#include <QVector>
#include <QTimer>

class IntroScreen : public QWidget {
    Q_OBJECT

public:
    explicit IntroScreen(QWidget *parent = nullptr);

signals:
    void backToMenuRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPixmap> wheatFrames;
    int currentFrameIndex = 0;
    QTimer *animTimer;

    void loadAssets();
    void setupUI();
};

#endif // INTROSCREEN_H