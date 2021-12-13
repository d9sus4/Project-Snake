#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <QWidget>
#include <QAction>
#include <QPushButton>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QtWidgets>
#include <QGroupBox>
#include "scene.h"

class Scene;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *);

protected slots:
    void gameEnded();

private:
    Ui::MainWindow *ui;
    Scene* scene;
    QGroupBox* ctrls;
    void updateCtrl();
    void startGame();
    void pauseGame();
    void resumeGame();
    void restartGame();
    void leaveGame();
    void saveGame();
    void loadGame();
    void loadData();
    void saveData();
    QAction *actions[7];
    QPushButton *buttons[7];
};

#endif // MAINWINDOW_H
