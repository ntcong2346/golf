#include "Reward.h"

Reward::Reward() : id(0), x(0.0), y(0.0), value(0) {}

Reward::Reward(int id, double x, double y, int value)
    : id(id), x(x), y(y), value(value) {}

int Reward::getId() const {
    return id;
}

double Reward::getX() const {
    return x;
}

double Reward::getY() const {
    return y;
}

int Reward::getValue() const {
    return value;
}

void Reward::setId(int newId) {
    id = newId;
}

void Reward::setX(double newX) {
    x = newX;
}

void Reward::setY(double newY) {
    y = newY;
}

void Reward::setValue(int newValue) {
    value = newValue;
}

void Reward::display() const {
    std::cout << "Reward ID: " << id << ", X: " << x << ", Y: " << y
              << ", Value: " << value << std::endl;
}
