#ifndef HOLE_H
#define HOLE_H

#include <vector>

// Forward declaration
class Obstacle;
class Reward;

class Hole {
    int id;
    double x, y, radius;
    std::vector<Obstacle> obstacles; // Sử dụng forward declaration
    std::vector<Reward> rewards;    // Sử dụng forward declaration

public:
    Hole(int id, double x, double y, double radius);
    void display() const;
};

class Game {
private:
    std::vector<Hole> holes;
    std::vector<Obstacle> obstacles;
    std::vector<Reward> rewards;
};

#endif // HOLE_H
