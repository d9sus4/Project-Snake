#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>

class Scene: public QWidget
{
    Q_OBJECT

public:
    Scene(QWidget *);
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void initScene();
    void fruitGenerator();

    enum State {Ready, Playing, Paused, Ended} state = Ended;
    enum Direction {Up, Down, Left, Right, Still} heading = Still, facing = Still;
    double time = 0.0;
    int distance = 0;
    int lenth = 0;
    int grow = 0;
    QList<std::pair<int, int>> snake;
    QVector<std::pair<int, int>> wall;
    QList<std::pair<int, int>> fruit;

    QTimer* frame_timer;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

protected slots:
    void timeOut();

signals:
    void gameOver();

private:
    QRectF makeRect(const std::pair<int, int> &);
    void updateTail();
    bool available(int, int);
    bool crash();
    bool score();

};
#endif // SCENE_H
