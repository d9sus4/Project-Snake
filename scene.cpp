#include "scene.h"
#include "consts.h"

Scene::Scene(QWidget *parent):QWidget(parent){
    lenth = INIT_LEN;
    frame_timer = new QTimer;
    connect(frame_timer, SIGNAL(timeout()),this,SLOT(timeOut()));
}

void Scene::initScene(){
    snake.clear();
    fruit.clear();
    wall.clear();
    snake.prepend({START_X, START_Y});
    grow = INIT_LEN - 1;
    lenth = INIT_LEN;
    if (INIT_WALL){
        for (int i = 0; i < SCALE - 1; ++i){
            wall.append({i, 0});
            wall.append({SCALE - 1, i});
            wall.append({SCALE - 1 - i, SCALE - 1});
            wall.append({0, SCALE - 1 - i});
        }
    }
    heading = Still;
    facing = Still;
    state = Ready;
    time = 0.0;
    distance = 0;
    update();
}

void Scene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPen pen;
    QBrush brush;
    QFont font("default", 12, QFont::ExtraLight, false);
    pen.setColor(Qt::black);
    brush.setColor(Qt::green);
    brush.setStyle(Qt::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    for (const auto &body: snake){
        painter.drawRect(makeRect(body));
    }
    brush.setColor(Qt::gray);
    painter.setBrush(brush);
    for (const auto &brick: wall){
        painter.drawRect(makeRect(brick));
    }
    brush.setColor(Qt::red);
    painter.setBrush(brush);
    for(const auto &food: fruit){
        painter.drawEllipse(makeRect(food));
    }
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawText(20, 30, QString("Time: ") + QString::number(time, 'f', 1) + QString("s"));
    painter.drawText(20, 45, QString("Distance: ") + QString::number(distance, 'f', 0));
    painter.drawText(20, 60, QString("Score: ") + QString("%1").arg(lenth - INIT_LEN));
}

void Scene::timeOut(){
    switch (heading){
    case Up:
        moveUp();
        break;
    case Down:
        moveDown();
        break;
    case Left:
        moveLeft();
        break;
    case Right:
        moveRight();
        break;
    case Still:
        break;
    }
    facing = heading;
}

void Scene::moveUp(){
    snake.prepend({snake.at(0).first, (snake.at(0).second + SCALE - 1) % SCALE});
    updateTail();
}

void Scene::moveDown(){
    snake.prepend({snake.at(0).first, (snake.at(0).second + 1) % SCALE});
    updateTail();
}

void Scene::moveLeft(){
    snake.prepend({(snake.at(0).first + SCALE - 1) % SCALE, snake.at(0).second});
    updateTail();
}

void Scene::moveRight(){
    snake.prepend({(snake.at(0).first + 1) % SCALE, snake.at(0).second});
    updateTail();
}

void Scene::updateTail(){
    if (grow == 0) {snake.removeLast();}
    else {--grow;}
    if (crash()){
        emit gameOver();
        return;
    }
    if (score()){
        fruitGenerator();
    }
    time += FRAME_TIME / 1000.0;
    ++distance;
    update();
}

bool Scene::crash(){
    for (const auto &obs: wall){
        if (obs == snake.at(0)) {return true;}
    }
    for (int i = 1; i < snake.size(); ++i){
        if (snake.at(i) == snake.at(0)) {return true;}
    }
    return false;
}

bool Scene::score(){
    for (int i = 0; i < fruit.size(); ++i){
        if (fruit.at(i) == snake.at(0)) {
            fruit.removeAt(i);
            grow += GROW_LEN;
            lenth += GROW_LEN;
            return true;
        }
    }
    return false;
}

void Scene::mousePressEvent(QMouseEvent *event){
    if (state != Ready || makeRect(snake.at(0)).contains(event->pos())) {return;}
    for (int i = 0; i < wall.size(); ++i){
        if (makeRect(wall.at(i)).contains(event->pos())) {
            wall.removeAt(i);
            update();
            return;
        }
    }
    int x = event->x() / TILE_SIZE;
    int y = event->y() / TILE_SIZE;
    wall.append({x, y});
    update();
}

QRectF Scene::makeRect(const std::pair<int, int> &coordinate){
    return QRectF(coordinate.first * TILE_SIZE, coordinate.second * TILE_SIZE, TILE_SIZE, TILE_SIZE);
}

bool Scene::available(int x, int y){
    for (const auto &brick: wall){
        if (brick.first == x && brick.second == y) {return false;}
    }
    for (const auto &body: snake){
        if (body.first == x && body.second == y) {return false;}
    }
    for (const auto &food: fruit){
        if (food.first == x && food.second == y) {return false;}
    }
    return true;
}

void Scene::fruitGenerator(){
    auto temp = std::make_pair(0, 0);
    while (true){
        int x = qrand() % SCALE, y = qrand() % SCALE;
        if (available(x, y)) {temp.first = x; temp.second = y; break;}
    }
    fruit.append(temp);
}
