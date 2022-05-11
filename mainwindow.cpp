#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGridLayout>
#include <QDebug>
#include <set>
#include <QThread>
#include <QMessageBox>
#include "windows.h"

#include "cell.h"

using namespace std;

#include <QTimer>
#include <QTime>

inline void delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

#include <fstream>

bool cmp(Cell* obj1, Cell* obj2) { return obj1->get_weight() < obj2->get_weight(); }
bool cmp_distance(Cell* obj1, Cell* obj2) { return obj1->get_distance_from_the_beginning() < obj2->get_distance_from_the_beginning(); }

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheNone);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setBackgroundBrush(QColor(255, 255, 255));

    scene.setSceneRect(0, 0, 800, 640);

    scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->graphicsView->setScene(&scene);

    int i,j;
    for (i = 0; i < 9; i++)
        for (j = 0; j < 11; j++)
        {
            cells[i][j] = nullptr;
        }

    init_map();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete_map();
}


// 1 - длина пути из начальной точки в текущую
// 2 - эвриситическое приближение(количество перемещений до конца, игнорируя диагональные переходы)
// 3 - вес ячейки(сумма из двух значений выше)

void MainWindow::on_pshBtn_AStar_clicked()
{
    ui->pshBtn_Dijkstra->setEnabled(false);
    ui->pshBtn_AStar->setEnabled(false);
    init_map();
    A_star();
    ui->pshBtn_Dijkstra->setEnabled(true);
    ui->pshBtn_AStar->setEnabled(true);
}

void MainWindow::A_star()
{
    multiset<Cell*, decltype (cmp)*> set_cells(cmp);

    Cell* current_cell = start_p;
    int i, j;

    while (current_cell != finish_p)
    {
        delay(100);
        current_cell->block();
        if (!set_cells.empty())
            set_cells.erase(set_cells.begin());
        current_cell->set_color(QColor(	0, 191, 255));
        current_cell->update();


        for (i = current_cell->get_X() - 1; i <= current_cell->get_X() + 1; i++)
        {
            for(j = current_cell->get_Y() - 1; j <= current_cell->get_Y() + 1; j++)
            {
                if (i < 0 || j < 0 || i > 9 || j > 7 || cells[i][j]->is_blocked())
                    continue;

                if (i == current_cell->get_X() || j == current_cell->get_Y())
                {
                    if (cells[i][j]->is_visited() && (current_cell->get_distance_from_the_beginning() + cells[i][j]->get_distance() >= cells[i][j]->get_distance_from_the_beginning()))
                        continue;

                    cells[i][j]->set_distance_from_the_beginning(current_cell->get_distance_from_the_beginning() + cells[i][j]->get_distance());
                }
                else
                {
                    if (cells[i][j]->is_visited() && ((current_cell->get_distance_from_the_beginning() + cells[i][j]->get_distance()*1.4) >= cells[i][j]->get_distance_from_the_beginning()))
                        continue;
                    cells[i][j]->set_distance_from_the_beginning(current_cell->get_distance_from_the_beginning() + cells[i][j]->get_distance()*1.4);
                }

                if (cells[i][j]->is_visited())
                {
                    qDebug() << set_cells.size();
                    set_cells.erase(cells[i][j]);
                    qDebug() << set_cells.size();
                    qDebug() << "-----------------";
                }

                cells[i][j]->visited();

                cells[i][j]->set_heuristic(heuristic(cells[i][j]));

                cells[i][j]->set_parent(current_cell);

                cells[i][j]->update_weight();

                cells[i][j]->set_type(Cell::CHECK);
                delay(100);

                set_cells.insert(cells[i][j]);

            }
        }
        current_cell->set_type(Cell::Status::CLOSED_LIST);
        current_cell = (*(set_cells.begin()));
        if (set_cells.empty())
            break;
    }
    if (finish_p->get_parent_coords().x() == -1)
    {
        QMessageBox MsgBox;
        MsgBox.setText("Путь до точки финиша не был найден!");
        MsgBox.exec();
    }
    else
    {
        start_p->set_color(QColor(127, 255, 0));
        finish_p->set_color(QColor(0, 255, 255));
        start_p->update();
        finish_p->update();
        final_path();
    }
}

void MainWindow::Dijkstra()
{

     Cell* current_cell = start_p;
     multiset<Cell*, decltype (cmp_distance)*> set_cells(cmp_distance);

     int i, j;

     while (1)
     {
         delay(100);
         current_cell->block();
         current_cell->set_type(Cell::CHECK);

         for (i = current_cell->get_X() - 1; i <= current_cell->get_X() + 1; i++)
         {
             for(j = current_cell->get_Y() - 1; j <= current_cell->get_Y() + 1; j++)
             {
                 if (i < 0 || j < 0 || i > 9 || j > 7 || cells[i][j]->is_blocked())
                     continue;

                 if (i == current_cell->get_X() || j == current_cell->get_Y())
                 {
                     if (cells[i][j]->is_visited() && (current_cell->get_distance_from_the_beginning() + cells[i][j]->get_distance() >= cells[i][j]->get_distance_from_the_beginning()))
                         continue;

                     cells[i][j]->set_distance_from_the_beginning(current_cell->get_distance_from_the_beginning() + cells[i][j]->get_distance());
                 }
                 else
                 {
                     if (cells[i][j]->is_visited() && ((current_cell->get_distance_from_the_beginning() + cells[i][j]->get_distance()*1.4) >= cells[i][j]->get_distance_from_the_beginning()))
                         continue;
                     cells[i][j]->set_distance_from_the_beginning(current_cell->get_distance_from_the_beginning() + cells[i][j]->get_distance()*1.4);
                 }

                 if (cells[i][j]->is_visited())
                     set_cells.erase(cells[i][j]);
                 cells[i][j]->visited();

                 cells[i][j]->set_parent(current_cell);

                 set_cells.insert(cells[i][j]);
             }
         }

         if (set_cells.empty())
             break;
         current_cell = (*(set_cells.begin()));
         set_cells.erase(set_cells.begin());
     }

    if (finish_p->get_parent_coords().x() == -1)
    {
        QMessageBox MsgBox;
        MsgBox.setText("Путь до точки финиша не был найден!");
        MsgBox.exec();
    }
    else
    {
        start_p->set_color(QColor(127, 255, 0));
        finish_p->set_color(QColor(0, 255, 255));
        start_p->update();
        finish_p->update();
        final_path();
    }
}

void MainWindow::delete_map()
{
    int i,j;
    for (i = 0; i < 9; i++)
        for (j = 0; j < 11; j++)
        {
            if (cells[i][j])
                delete cells[i][j];
            cells[i][j] = nullptr;
        }
}


void MainWindow::final_path()
{
    Cell* current_cell = finish_p;

    QGraphicsLineItem* line;
    while (current_cell != start_p)
    {
        line = new QGraphicsLineItem();
        line->setPen(QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        scene.addItem(line);
        line->setLine(current_cell->get_X()*LENGHT + LENGHT / 2,
                      current_cell->get_Y() * LENGHT + LENGHT / 2,
                      current_cell->get_parent_coords().x()* LENGHT + LENGHT/2,
                      current_cell->get_parent_coords().y() * LENGHT + LENGHT / 2);
        current_cell = cells[current_cell->get_parent_coords().x()][current_cell->get_parent_coords().y()];
    }

}

void MainWindow::on_pshBtn_Dijkstra_clicked()
{
    ui->pshBtn_AStar->setEnabled(false);
    ui->pshBtn_Dijkstra->setEnabled(false);
    init_map();

    Dijkstra();
    ui->pshBtn_AStar->setEnabled(true);
    ui->pshBtn_Dijkstra->setEnabled(true);
}

void MainWindow::init_map()
{
    delete_map();
    std::ifstream f_map("C:\\qt_project\\laba1_sem4\\map.txt");
    char c;

    if (!f_map.is_open())
    {
        qDebug() << "Файл не открылся!";
    }

    int i;
    int j;
    for (i = 0; i < 9; i++)
        for (j = 0; j < 11; j++)
        {
            c = f_map.get();

            if (c == '\n')
                continue;

            cells[j][i] = new Cell(j,i, c);
            if (c == 'S')
                start_p = cells[j][i];
            else if (c == 'F')
                finish_p = cells[j][i];

            scene.addItem(cells[j][i]);

        }
}

int MainWindow::heuristic(Cell* cell)
{
    return (abs(cell->get_X() - finish_p->get_X()) + abs(cell->get_Y() - finish_p->get_Y()))*10;
}


