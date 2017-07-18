#include "position.h"

Pos::Pos() : x_val(0), y_val(0) {}
Pos::~Pos() {}

int Pos::x() const { return x_val; }
int Pos::y() const { return y_val; }
int Pos::index() const { return x_val*8+y_val; }
void Pos::operator=(const Pos& Q){ x_val = Q.x_val; y_val = Q.y_val; }
Pos::Pos(int _x, int _y) : x_val(_x), y_val(_y) {}
Pos::Pos(const Pos &position) : x_val(position.x()), y_val(position.y()) {}
void Pos::setX(int _x) { x_val = _x; }
void Pos::setY(int _y) { y_val = _y; }
void Pos::set(int _x, int _y) { x_val = _x; y_val = _y; }
void Pos::set(const Pos& position) {
    x_val = position.x();
    y_val = position.y();
}	
