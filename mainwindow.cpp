#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "consts.h"
#include "scene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Project Snake by @Delphynium"));
    scene = new Scene(this);
    scene->setFixedSize(MAP_SIZE, MAP_SIZE);
    scene->setGeometry(QRect(0, ui->mainToolBar->height(), MAP_SIZE, MAP_SIZE));
    ui->mainToolBar->setMovable(false);

    ctrls = new QGroupBox(tr("Game Controls"), this);
    buttons[0] = new QPushButton(tr("Start"));
    buttons[1] = new QPushButton(tr("Pause"));
    buttons[2] = new QPushButton(tr("Resume"));
    buttons[3] = new QPushButton(tr("Restart"));
    buttons[4] = new QPushButton(tr("Leave"));
    buttons[5] = new QPushButton(tr("Save"));
    buttons[6] = new QPushButton(tr("Load"));
    QVBoxLayout *vbox = new QVBoxLayout;
    for (auto *button: buttons){
        vbox->addWidget(button);
    }
    ctrls->setLayout(vbox);
    ctrls->setGeometry(QRect(MAP_SIZE, ui->mainToolBar->height(), 100 ,MAP_SIZE));

    this->setFixedSize(MAP_SIZE + 100, MAP_SIZE + ui->mainToolBar->height() + ui->statusBar->height());

    actions[0] = new QAction(tr("Start"));
    actions[1] = new QAction(tr("Pause"));
    actions[2] = new QAction(tr("Resume"));
    actions[3] = new QAction(tr("Restart"));
    actions[4] = new QAction(tr("Leave"));
    actions[5] = new QAction(tr("Save"));
    actions[6] = new QAction(tr("Load"));

    connect(actions[0], &QAction::triggered, this, &MainWindow::startGame);
    connect(actions[1], &QAction::triggered, this, &MainWindow::pauseGame);
    connect(actions[2], &QAction::triggered, this, &MainWindow::resumeGame);
    connect(actions[3], &QAction::triggered, this, &MainWindow::restartGame);
    connect(actions[4], &QAction::triggered, this, &MainWindow::leaveGame);
    connect(actions[5], &QAction::triggered, this, &MainWindow::saveData);
    connect(actions[6], &QAction::triggered, this, &MainWindow::loadData);

    connect(buttons[0], &QPushButton::clicked, this, &MainWindow::startGame);
    connect(buttons[1], &QPushButton::clicked, this, &MainWindow::pauseGame);
    connect(buttons[2], &QPushButton::clicked, this, &MainWindow::resumeGame);
    connect(buttons[3], &QPushButton::clicked, this, &MainWindow::restartGame);
    connect(buttons[4], &QPushButton::clicked, this, &MainWindow::leaveGame);
    connect(buttons[5], &QPushButton::clicked, this, &MainWindow::saveData);
    connect(buttons[6], &QPushButton::clicked, this, &MainWindow::loadData);

    QMenu *menu_0 = ui->menuBar->addMenu(tr("Game Controls"));
    for (QAction *action: actions){
        ui->mainToolBar->addAction(action);
        menu_0->addAction(action);
    }

    scene->initScene();
    updateCtrl();
    connect(scene, SIGNAL(gameOver()), this, SLOT(gameEnded()));
}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::startGame(){
    scene->state = Scene::State::Playing;
    updateCtrl();
    scene->fruitGenerator();
    update();
}

void MainWindow::pauseGame(){
    scene->frame_timer->stop();
    scene->state = Scene::State::Paused;
    updateCtrl();
}

void MainWindow::resumeGame(){
    scene->frame_timer->start(FRAME_TIME);
    scene->state = Scene::State::Playing;
    updateCtrl();
}

void MainWindow::restartGame(){
    scene->frame_timer->stop();
    scene->initScene();
    scene->state = Scene::State::Ready;
    updateCtrl();
}

void MainWindow::leaveGame(){
    qApp->exit();
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if (scene->state != Scene::State::Playing) {return;}
    switch (event->key()){
    case Qt::Key_Up: case Qt::Key_W:
        if (scene->facing != Scene::Direction::Down) {
            scene->heading = Scene::Direction::Up;
        }
        break;
    case Qt::Key_Down: case Qt::Key_S:
        if (scene->facing != Scene::Direction::Up) {
            scene->heading = Scene::Direction::Down;
        }
        break;
    case Qt::Key_Left: case Qt::Key_A:
        if (scene->facing != Scene::Direction::Right) {
            scene->heading = Scene::Direction::Left;
        }
        break;
    case Qt::Key_Right: case Qt::Key_D:
        if (scene->facing != Scene::Direction::Left) {
            scene->heading = Scene::Direction::Right;
        }
        break;
    }
    if (!scene->frame_timer->isActive())
        scene->frame_timer->start(FRAME_TIME);
}

void MainWindow::updateCtrl(){
    switch (scene->state){
    case 0:
        for (int i = 0; i <= 6; ++i){
            actions[i]->setEnabled(true);
            buttons[i]->setEnabled(true);
        }
        for (int i: {1,2,3,5}){
            actions[i]->setEnabled(false);
            buttons[i]->setEnabled(false);
        }
        break;
    case 1:
        for (int i = 0; i <= 6; ++i){
            actions[i]->setEnabled(true);
            buttons[i]->setEnabled(true);
        }
        for (int i: {0,2,3,5,6}){
            actions[i]->setEnabled(false);
            buttons[i]->setEnabled(false);
        }
        break;
    case 2:
        for (int i = 0; i <= 6; ++i){
            actions[i]->setEnabled(true);
            buttons[i]->setEnabled(true);
        }
        for (int i: {0,1,6}){
            actions[i]->setEnabled(false);
            buttons[i]->setEnabled(false);
        }
        break;
    case 3:
        for (int i = 0; i <= 6; ++i){
            actions[i]->setEnabled(true);
            buttons[i]->setEnabled(true);
        }
        for (int i: {0,1,2,5,6}){
            actions[i]->setEnabled(false);
            buttons[i]->setEnabled(false);
        }
        break;
    }
}

void MainWindow::gameEnded(){
    scene->frame_timer->stop();
    scene->state = Scene::Ended;
    updateCtrl();
}

void MainWindow::saveData(){
    QString path = QFileDialog::getSaveFileName(this, tr("Open Config"), "data.snake", tr("Project Snake Data(*.snake)"));
    if (!path.isNull()){
        QFile data_file(path);
        if(!data_file.open(QIODevice::WriteOnly)) {return;}
        QTextStream ts(&data_file);
        ts << scene->state << '\n'
           << scene->heading << '\n'
           << scene->facing << '\n'
           << scene->time << '\n'
           << scene->distance << '\n'
           << scene->lenth << '\n'
           << scene->grow << '\n';
        ts << scene->snake.size() << '\n';
        for (int i = 0; i < scene->snake.size(); ++i){
            ts << scene->snake.at(i).first << ' ' << scene->snake.at(i).second << '\n';
        }
        ts << scene->wall.size() << '\n';
        for (auto &brick: scene->wall){
            ts << brick.first << ' ' << brick.second << '\n';
        }
        ts << scene->fruit.size() << '\n';
        for (auto &food: scene->fruit){
            ts << food.first << ' ' << food.second << '\n';
        }
        data_file.close();
    }
}

void MainWindow::loadData(){
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), "", "Project Snake Data(*.snake)", nullptr);
    if (!path.isNull()){
        QFile data_file(path);
        if(!data_file.open(QIODevice::ReadOnly)) {return;}
        scene->initScene();
        QTextStream ts(&data_file);
        int temp, temp_1, temp_2;
        ts >> temp;
        switch (temp) {
        case 0:
            scene->state = Scene::Ready;
            break;
        case 1:
            scene->state = Scene::Playing;
            break;
        case 2:
            scene->state = Scene::Paused;
            break;
        case 3:
            scene->state = Scene::Ended;
            break;
        }
        updateCtrl();
        ts >> temp;
        switch (temp) {
        case 0:
            scene->heading = Scene::Up;
            break;
        case 1:
            scene->heading = Scene::Down;
            break;
        case 2:
            scene->heading = Scene::Left;
            break;
        case 3:
            scene->heading = Scene::Right;
            break;
        case 4:
            scene->heading = Scene::Still;
            break;
        }
        ts >> temp;
        switch (temp) {
        case 0:
            scene->facing = Scene::Up;
            break;
        case 1:
            scene->facing = Scene::Down;
            break;
        case 2:
            scene->facing = Scene::Left;
            break;
        case 3:
            scene->facing = Scene::Right;
            break;
        case 4:
            scene->facing = Scene::Still;
            break;
        }
        ts >> scene->time
           >> scene->distance
           >> scene->lenth
           >> scene->grow;
        ts >> temp;
        scene->snake.clear();
        for (int i = 0; i < temp; ++i){
            ts >> temp_1 >> temp_2;
            scene->snake.append(std::make_pair(temp_1, temp_2));
        }
        ts >> temp;
        scene->wall.clear();
        for (int i = 0; i < temp; ++i){
            ts >> temp_1 >> temp_2;
            scene->wall.append(std::make_pair(temp_1, temp_2));
        }
        ts >> temp;
        scene->fruit.clear();
        for (int i = 0; i < temp; ++i){
            ts >> temp_1 >> temp_2;
            scene->fruit.append(std::make_pair(temp_1, temp_2));
        }
        data_file.close();
    }
}
