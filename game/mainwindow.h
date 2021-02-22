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
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QStringList>


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
    void load_newMap ();

protected:
    void paintEvent (QPaintEvent *e);


public slots:
    void on_exit_button_clicked();
    void on_load_new_map_clicked();
    void on_reload_map_clicked();
    void on_load_prev_map_clicked();
    void on_level_1_clicked();
    void on_level_2_clicked();
    void on_level_3_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap back_wall;//0
    QPixmap wall;//1
    QPixmap player;//2
    QPixmap box;//3
    QPixmap point;//4
    QPixmap ready_box;//5

    QPixmap winner;
    QPainter pnt;
    bool gameover();
    void win();
    void load_file();
    static const int BOX_WIDTH = 400;
    static const int BOX_HEIGHT = 400;
    static const int DOT_SIZE = 50;
    int counter = 0;
};
#endif // MAINWINDOW_H
