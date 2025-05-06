
// Renderer.cpp
#include "C:\Users\Cong\golf\src\Render.h" // Adjust the path to the correct location of Renderer.h

void Renderer::renderHole(const Hole& hole) {
    std::cout << "Rendering Hole: ";
    hole.display();
}

void Renderer::renderObstacle(const Obstacle& obstacle) {
    std::cout << "Rendering Obstacle: ";
    obstacle.display();
}

void Renderer::renderReward(const Reward& reward) {
    std::cout << "Rendering Reward: ";
    reward.display();
}