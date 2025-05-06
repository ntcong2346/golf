#ifndef HOLE_H
#define HOLE_H

#include "Hole.h"
#include <iostream>

class Hole {
public:
    Hole();
    Hole(int id, double x, double y, double radius);
    int getId() const;
    double getX() const;
    double getY() const;
    double getRadius() const;
    void setId(int newId);
    void setX(double newX);
    void setY(double newY);
    void setRadius(double newRadius);
    void display() const;

private:
    int id;
    double x, y, radius;
};

#endif // HOLE_H

Hole::Hole() : id(0), x(0.0), y(0.0), radius(0.0) {}

Hole::Hole(int id, double x, double y, double radius)
    : id(id), x(x), y(y), radius(radius) {}

int Hole::getId() const {
    return id;
}

double Hole::getX() const {
    return x;
}

double Hole::getY() const {
    return y;
}

double Hole::getRadius() const {
    return radius;
}

void Hole::setId(int newId) {
    id = newId;
}

void Hole::setX(double newX) {
    x = newX;
}

void Hole::setY(double newY) {
    y = newY;
}

void Hole::setRadius(double newRadius) {
    radius = newRadius;
}

void Hole::display() const {
    std::cout << "Hole ID: " << id << ", X: " << x
              << ", Y: " << y << ", Radius: " << radius << std::endl;
}
