#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <iostream>

class Obstacle {
private:
    int id;
    double x;
    double y;
    int width;
    int height;

public:
    Obstacle();
    Obstacle(int id, double x, double y);
    Obstacle(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {}

    int getId() const;
    double getX() const;
    double getY() const;

    void setId(int newId);
    void setX(double newX);
    void setY(double newY);

    void display() const;
};

#endif // OBSTACLE_H
