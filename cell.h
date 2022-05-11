#ifndef CELL_H
#define CELL_H

#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <utility>

#define LENGHT 80

class Cell : public QGraphicsItem
{
public:
    Cell();
    Cell(short int x, short int y, char c);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void set_color(QColor color);

    bool is_blocked();
    void block();
    void visited();
    bool is_visited();


    void update_weight();

    enum Status{
        BARRIER,
        USUAL,
        FOREST,
        WATER,
        CLOSED_LIST,
        CHECK,
        START,
        FINISH
               };
    void set_type(int st);

    short int get_X();
    short int get_Y();

    void set_distance_from_the_beginning(int x);
    int  get_distance_from_the_beginning();
    int get_weight();
    int get_distance();
    void set_heuristic(int heuristic);


    void set_parent(Cell* pr);
    const QPoint& get_parent_coords();

private:
    QColor color = QColor(255,250,250); // тут сделать что-нибудь помягче чем зелёный

    int distance_from_the_beginning{0};
    int heuristic_approximation{0};
    int weight{0};

    char status{0}; // записывать была ли посещена ячейка или нет

    int distance{10};

    QPoint parent{-1,-1};

    QPixmap pix_arrow;

    short int type{USUAL}; // 0 - препядствие, 1 - обычная ячейка, 2 - начало, 3 - конец, 3 - путь

    short int X;
    short int Y;

};

#endif // CELL_H
