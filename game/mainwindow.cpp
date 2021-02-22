#include "mainwindow.h"
#include "levels.h"
#include "ui_mainwindow.h"
#include <QGraphicsItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player.load("D:/pic/people.png");
    wall.load("D:/pic/wall.png");
    box.load("D:/pic/box.png");
    point.load("D:/pic/checkpoint.png");
    ready_box.load("D:/pic/ready.png");
    back_wall.load("D:/pic/back_wall.png");
    winner.load ("D:/pic/winner.png");
    update();
    setFocus();
    setFocusPolicy(Qt::StrongFocus);

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_level_1_clicked(){
    loadmap(1);
    update();
}

void MainWindow::on_level_2_clicked(){
    loadmap(2);
    update();
}

void MainWindow::on_level_3_clicked(){
    loadmap(3);
    update();
}

void MainWindow::on_exit_button_clicked(){
    close();
}



void MainWindow::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);
    QPainter p(this);
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
         {
            switch (map[i][j])
            {
            case 1: p.drawPixmap(i*50+60,j*50+40,50,50,wall); break;
            case 2: p.drawPixmap(i*50+60,j*50+40,50,50,player); break;
            case 3: p.drawPixmap(i*50+60,j*50+40,50,50,box); break;
            //case 4: p.drawPoint(i*50+60,j*50+40);
            case 4: p.drawPixmap(i*50+60,j*50+40,50,50,point); break;
            case 5: p.drawPixmap(i*50+60,j*50+40,50,50,ready_box); break;
            case 0: p.drawPixmap(i*50+60,j*50+40,50,50,back_wall); break;
            case 9: p.drawPixmap(60,40,400,400,winner); break;
            }
        }
        update();

    }

}


void MainWindow::keyPressEvent(QKeyEvent *event){
    if (event->key()!= Qt::Key_W && event->key()!= Qt::Key_A && event->key()!= Qt::Key_S && event->key()!= Qt::Key_D)
        return;
    int ourKey = event->key();
    moves(ourKey);

}

void MainWindow::moves (int ourKey)
{
    int i,j;
    int gamer;
    for ( i = 0; i < 8; ++i){
        for (j = 0; j < 8; ++j)
            if (map[i][j] == 2) {
                gamer = map[i][j];
                break;
            }

        if (gamer == 2) break;
    }

    // 1 wall 2 player 3 box 4 check 5 ready box 0 back_wall

    switch(ourKey)
    {
    case Qt::Key_D:
        if (map[i+1][j] == 0) { // впереди пустота
            map[i][j] = 0;
            map [i+1][j] = gamer;
        }
        if (map[i+1][j] == 3 && map[i+2][j] == 0){//впереди бокс и за ним ничего
            map[i][j] = 0;
            map [i+1][j] = gamer;
            map [i+2][j] = 3;
        }
        if (map [i+1][j] == 3 && map [i+2][j] == 4){// впереди бокс и за ним чекпоинт
            map[i][j] = 0;
            map[i+1][j] = gamer;
            map[i+2][j] = 5;
        }

        break;
    case Qt::Key_A:
        if (map[i-1][j] == 0) {
            map[i][j] = 0;
            map [i-1][j] = gamer;
        }
        if (map[i-1][j] == 3 && map[i-2][j] == 0){
            map[i][j] = 0;
            map [i-1][j] = gamer;
            map [i-2][j] = 3;
        }
        if (map [i-1][j] == 3 && map [i-2][j] == 4){
            map[i][j] = 0;
            map[i-1][j] = gamer;
            map[i-2][j] = 5;
        }

        break;
    case Qt::Key_S:
        if (map[i][j+1] == 0) {
            map[i][j] = 0;
            map [i][j+1] = gamer;
        }
        if (map[i][j+1] == 3 && map[i][j+2] == 0){
            map[i][j] = 0;
            map [i][j+1] = gamer;
            map [i][j+2] = 3;
        }
        if (map [i][j+1] == 3 && map [i][j+2] == 4){
            map[i][j] = 0;
            map[i][j+1] = gamer;
            map[i][j+2] = 5;
        }

        break;
    case Qt::Key_W:
        if (map[i][j-1] == 0) {
            map[i][j] = 0;
            map [i][j-1] = gamer;
        }
        if (map[i][j-1] == 3 && map[i][j-2] == 0){
            map[i][j] = 0;
            map [i][j-1] = gamer;
            map [i][j-2] = 3;
        }
        if (map [i][j-1] == 3 && map [i][j-2] == 4){
            map[i][j] = 0;
            map[i][j-1] = gamer;
            map[i][j-2] = 5;
        }

        break;
    }
    update();
    if (gameover())
        win();
}
bool MainWindow::gameover()
{
    for(int i=0;i<8;++i)
        for(int j=0;j<8;++j)
            if(map[i][j]==3)
                return false;
    return true;
}

void MainWindow::win()
{
    loadmap(4);
}

void MainWindow::loadmap(int m){
    Levels tmp;
    for  (int j=0; j<8; j++){
        for (int i=0; i<8; i++){
            map [i][j] = tmp.level[m-1][j][i];
        }
    }
}

void MainWindow::load_file(){
    QFile file ("D:/pic/new_levels.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream in (&file);
    QString str = "";
    if (counter>0){
        file.seek(counter*82);
    }
    if (!in.atEnd()){
        for (int i = 0; i < 8; i++) {
            str = in.readLine();
            for (int j = 0; j < 8; j++){
                map[j][i] =  str[j].unicode() - 48;
            }
        }
    }
    counter++;
    file.close();
}

void MainWindow::load_newMap(){
    load_file();
}
void MainWindow::on_load_new_map_clicked(){
    load_newMap();
}
void MainWindow::on_reload_map_clicked(){
    counter --;
    load_newMap();
}
void MainWindow::on_load_prev_map_clicked(){
    if (counter>0)
    {
    counter -=2;
    }
    load_newMap();
}









