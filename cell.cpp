#include "cell.h"


Cell::Cell()
{

}

Cell::Cell(short int x, short int y, char c)
{
    X = x;
    Y = y;

//    parent.setX(x);
//    parent.setY(y);

    setPos(x*LENGHT, y*LENGHT);

    switch (c) {
    case 'b':
        set_type(Status::BARRIER);
        break;
    case 'S':
        set_type(Status::START);
        break;
    case 'F':
        set_type(Status::FINISH);
        break;
    case 'w':
        set_type(Status::WATER);
        break;
    case 'l':
        set_type(Status::FOREST);
        break;
    }
}

QRectF Cell::boundingRect() const
{
    return QRectF(0, 0, LENGHT, LENGHT);
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);

    painter->setBrush(color);
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawRect(0,0,LENGHT,LENGHT);

    if (type == Status::CLOSED_LIST || type == Status::CHECK)
    {
        painter->drawText(55, 60, QString::number(heuristic_approximation));
        painter->drawText(15, 60, QString::number(distance_from_the_beginning));
        painter->drawText(15, 10, QString::number(weight));

        painter->translate(40,40);

        painter->rotate(((get_X() - parent.x()) < 0 ? (0 + (parent.y() - get_Y())*45) : ( (get_X() - parent.x()) > 0) ? (180 - (parent.y() - get_Y())*45) : (parent.y() - get_Y())*90));
        painter->translate(-40,-40);
        painter->drawPixmap(30,30, 20, 20, pix_arrow, 0, 0, 142, 120);

    }
}

void Cell::set_color(QColor color)
{
    this->color = color;
}

bool Cell::is_blocked()
{
    return status == 2;
}

void Cell::block()
{
    status = 2;
}

void Cell::visited()
{
    status = 1;
}

bool Cell::is_visited()
{
    return status == 1;
}

void Cell::update_weight()
{
    weight = distance_from_the_beginning + heuristic_approximation;
}

void Cell::set_type(int st)
{
    type = st;

    switch (st) {
    case Status::BARRIER:
        color = QColor(48,48,48);
        block();
        break;
    case Status::START:
        color = QColor(0, 255, 255);
        break;

    case Status::FINISH:
        color = QColor(124, 252, 0);
        break;
    case Status::CLOSED_LIST:
        color = QColor(47, 79, 79);
        break;
    case Status::CHECK:
        color = QColor(color.darker());
        pix_arrow.load(":/arrow.png");
        break;
    case Status::FOREST:
        color = QColor(	34, 139, 34);
        distance = 20;
        break;
    case Status::WATER:
        color = QColor(	0, 191, 255);
        distance = 30;
        break;

    }
    update();
}

short int Cell::get_X()
{
    return X;
}

short Cell::get_Y()
{
    return Y;
}

void Cell::set_distance_from_the_beginning(int x)
{
    distance_from_the_beginning = x;
}

int Cell::get_distance_from_the_beginning()
{
    return distance_from_the_beginning;
}

int Cell::get_weight()
{
    return weight;
}

int Cell::get_distance()
{
    return distance;
}

void Cell::set_heuristic(int heuristic)
{
    heuristic_approximation = heuristic;
}

void Cell::set_parent(Cell* pr)
{
    parent.setX(pr->get_X());
    parent.setY(pr->get_Y());
}


const QPoint &Cell::get_parent_coords()
{
    return parent;
}
