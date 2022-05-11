#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "cell.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_pshBtn_AStar_clicked();

    void on_pshBtn_Dijkstra_clicked();


private:

    void A_star();
    void Dijkstra();

    Ui::MainWindow *ui;

    Cell* cells[16][12];

    void delete_map();
    void init_map();

    void final_path();

    int heuristic(Cell* cell);

    Cell* start_p;
    Cell* finish_p;

    QGraphicsScene scene;
};
#endif // MAINWINDOW_H
