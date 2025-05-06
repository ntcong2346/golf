#include "Obstacle.h"

Obstacle::Obstacle() : id(0), x(0.0), y(0.0) {}

Obstacle::Obstacle(int id, double x, double y) : id(id), x(x), y(y) {}

int Obstacle::getId() const {
    return id;
}

double Obstacle::getX() const {
    return x;
}

double Obstacle::getY() const {
    return y;
}

void Obstacle::setId(int newId) {
    id = newId;
}

void Obstacle::setX(double newX) {
    x = newX;
}

void Obstacle::setY(double newY) {
    y = newY;
}

void Obstacle::display() const {
    std::cout << "Obstacle ID: " << id << ", X: " << x << ", Y: " << y << std::endl;
}
