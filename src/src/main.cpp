#include "C:\Users\Cong\golf\src\include\SDL2\SDL.h"
#include "C:\Users\Cong\golf\src\include\SDL2\SDL_image.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath> 
#undef main

using namespace std;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL không thể khởi tạo! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    // Khởi tạo SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cout << "SDL_image không thể khởi tạo! SDL_image Error: " << IMG_GetError() << endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Quả bóng kéo thả", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Không thể tạo cửa sổ! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Không thể tạo renderer! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    int ballRadius = 15; // Bán kính quả bóng nhỏ hơn
    const int initialBallX = SCREEN_WIDTH / 2;
    const int initialBallY = SCREEN_HEIGHT / 2;
    int ballX = initialBallX;
    int ballY = initialBallY;
    float ballSpeedX = 0, ballSpeedY = 0;

    // Định nghĩa hố màu đỏ
    srand(static_cast<unsigned>(time(0)));
    int holeRadius = 20; // Bán kính hố
    int holeX = rand() % (SCREEN_WIDTH - 2 * holeRadius) + holeRadius;
    int holeY = rand() % (SCREEN_HEIGHT - 2 * holeRadius) + holeRadius;

    bool isDragging = false;
    int startX = 0, startY = 0; // Vị trí bắt đầu kéo

    bool quit = false;
    SDL_Event e;

    // Load ảnh
    SDL_Surface* ballSurface = IMG_Load("assets/ball.png");
    SDL_Surface* bgSurface = IMG_Load("assets/bg.png");
    SDL_Surface* holeSurface = IMG_Load("assets/hole.png");

    if (!ballSurface || !bgSurface || !holeSurface) {
        cout << "Không thể load ảnh! SDL_image Error: " << IMG_GetError() << endl;
        return -1;
    }

    // Tạo texture
    SDL_Texture* ballTexture = SDL_CreateTextureFromSurface(renderer, ballSurface);
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_Texture* holeTexture = SDL_CreateTextureFromSurface(renderer, holeSurface);

    // Giải phóng surfaces vì đã có textures
    SDL_FreeSurface(ballSurface);
    SDL_FreeSurface(bgSurface);
    SDL_FreeSurface(holeSurface);

    // Kích thước của ball và hole
    SDL_Rect ballRect = { SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 30, 30 }; // Điều chỉnh kích thước phù hợp
    SDL_Rect holeRect = { holeX, holeY, 40, 40 }; // Điều chỉnh kích thước phù hợp

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                // Kiểm tra nếu chuột click vào quả bóng
                int dx = e.button.x - ballX;
                int dy = e.button.y - ballY;
                if (sqrt(dx * dx + dy * dy) <= ballRadius) {
                    isDragging = true;
                    startX = e.button.x;
                    startY = e.button.y;
                }
            }

            if (e.type == SDL_MOUSEMOTION && isDragging) {
                // Di chuyển quả bóng khi kéo
                ballX = e.motion.x;
                ballY = e.motion.y;
            }

            if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                // Thả chuột, bắt đầu bay theo hướng kéo
                isDragging = false;

                // Tính vận tốc dựa trên khoảng cách kéo
                int endX = e.button.x;
                int endY = e.button.y;
                ballSpeedX = (endX - startX) / 8.0f; // Chia nhỏ để giảm tốc độ
                ballSpeedY = (endY - startY) / 8.0f;
            }
        }

        // Di chuyển quả bóng
        ballX += ballSpeedX;
        ballY += ballSpeedY;

        // Giảm dần vận tốc (ma sát cao hơn)
        ballSpeedX *= 0.95f;
        ballSpeedY *= 0.95f;

        // Nếu vận tốc rất nhỏ, dừng quả bóng
        if (fabs(ballSpeedX) < 0.1f) ballSpeedX = 0;
        if (fabs(ballSpeedY) < 0.1f) ballSpeedY = 0;

        // Kiểm tra nếu quả bóng ra ngoài cửa sổ, nếu có, làm quả bóng phản xạ
        if (ballX - ballRadius < 0 || ballX + ballRadius > SCREEN_WIDTH) {
            ballSpeedX = -ballSpeedX * 0.7f; // Phản xạ và giảm tốc
            ballX = max(ballRadius, min(ballX, SCREEN_WIDTH - ballRadius));
        }
        if (ballY - ballRadius < 0 || ballY + ballRadius > SCREEN_HEIGHT) {
            ballSpeedY = -ballSpeedY * 0.7f; // Phản xạ và giảm tốc
            ballY = max(ballRadius, min(ballY, SCREEN_HEIGHT - ballRadius));
        }

        // Kiểm tra nếu quả bóng rơi vào hố
        int dx = ballX - holeX;
        int dy = ballY - holeY;
        if (sqrt(dx * dx + dy * dy) <= ballRadius + holeRadius) {
            // Đặt lại vị trí quả bóng về vị trí ban đầu
            ballX = initialBallX;
            ballY = initialBallY;
            ballSpeedX = 0;
            ballSpeedY = 0;

            // Tạo vị trí mới cho hố
            holeX = rand() % (SCREEN_WIDTH - 2 * holeRadius) + holeRadius;
            holeY = rand() % (SCREEN_HEIGHT - 2 * holeRadius) + holeRadius;
        }

        // Cập nhật vị trí của ballRect và holeRect
        ballRect.x = ballX - ballRect.w/2;
        ballRect.y = ballY - ballRect.h/2;
        holeRect.x = holeX - holeRect.w/2;
        holeRect.y = holeY - holeRect.h/2;

        // Render background
        SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
        
        // Render hole
        SDL_RenderCopy(renderer, holeTexture, NULL, &holeRect);
        
        // Render ball
        SDL_RenderCopy(renderer, ballTexture, NULL, &ballRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    // Giải phóng textures
    SDL_DestroyTexture(ballTexture);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(holeTexture);

    // Giải phóng tài nguyên
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit(); // Thêm dòng này trước SDL_Quit()
    SDL_Quit();

    return 0;
}