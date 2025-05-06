#ifndef REWARD_H
#define REWARD_H

#include <iostream>

class Reward {
public:
    Reward(int id, double x, double y, int value);
    void display() const;

private:
    int id;
    double x, y; // Giữ kiểu dữ liệu double cho x và y
    int value;
};

#endif // REWARD_H
