#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QKeyEvent>
#include <QPixmap>
#include <QImage>
#include<QGraphicsItem>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int map[8][8]{
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}  };
    void loadmap (int m);
    void moves(int ourKey);
    void keyPressEvent(QKeyEvent *event);
protected:
    void paintEvent (QPaintEvent *e);


public slots:
    void on_exit_button_clicked();
    void on_level_1_clicked();
    void on_level_2_clicked();
    void on_level_3_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap player;
    QPixmap wall;
    QPixmap box;
    QPixmap point;
    QPixmap ready_box;
    QPixmap back_wall;

    QPixmap winner;
    bool gameover();
    void win();

    static const int BOX_WIDTH = 400;
    static const int BOX_HEIGHT = 400;
    static const int DOT_SIZE = 50;
};
#endif // MAINWINDOW_H
