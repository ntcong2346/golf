# Let’s Golf - Dual Mini Golf

![Main Menu]
![rm1](https://github.com/user-attachments/assets/b5410b22-3ca2-4704-bc8d-63fdb210cdc3)

*Màn hình menu chính*

## Giới thiệu
Đây là game mini golf đối kháng 2 người chơi được phát triển bằng thư viện SDL2 (C++). Người chơi lần lượt đánh bóng vào lỗ với số gậy ít nhất để giành chiến thắng.

## Tính năng chính
- 🎮 Chế độ chơi: 2 người chơi trên cùng thiết bị
- ⛳ 4 màn chơi với chướng ngại vật độc đáo
- 🏆 Hệ thống tính điểm và xếp hạng
- ⚙️ Tuỳ chỉnh tốc độ bóng và âm thanh
- 💝 Phần thưởng giảm số gậy khi nhặt vật phẩm
## Game Screenshots

![Gameplay]
![rm3](https://github.com/user-attachments/assets/a081fb9a-5f0c-4572-80f9-aeaa4667a7dc)

*Giao diện gameplay với thông tin số gậy và round*

![Game Over]
![Ảnh chụp màn hình 2025-05-08 002330](https://github.com/user-attachments/assets/61bfffec-b0fb-4f10-b8c0-15bf1bf200b1)

*Màn hình kết thúc game hiển thị người chiến thắng*

## Cài đặt

### Yêu cầu hệ thống
- Hệ điều hành: Windows/Linux/macOS
- Thư viện cần thiết:
  - SDL2
  - SDL2_image
  - SDL2_ttf
  - SDL2_mixer

### Yêu cầu hệ thống
- Hệ điều hành: Windows/Linux/macOS
- Thư viện cần thiết:
  - SDL2
  - SDL2_image
  - SDL2_ttf
  - SDL2_mixer

### Cài đặt trên Windows
1. Cài đặt [MSYS2](https://www.msys2.org/)
2. Cài đặt các thư viện cần thiết:
   ```bash
   pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_mixer
Clone repository:

bash
git clone https://github.com/yourusername/golf-game.git
cd golf-game
Biên dịch và chạy:

bash
g++ -std=c++17 main.cpp init.cpp update.cpp render.cpp -o golf_game -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
./golf_game
Hướng dẫn chơi
Nhấn chuột trái vào bóng và kéo để chỉnh hướng
Thả chuột để đánh bóng

Thanh năng lượng hiển thị lực đánh

Mục tiêu: Đưa bóng vào lỗ với số gậy ít nhất

Nhặt vật phẩm trái tim để được giảm 1 gậy

Điều khiển
Phím	Chức năng
ESC	Thoát game
Space	Quay lại menu
Mouse	Kéo/thả để đánh bóng

Cấu trúc thư mục


golf/
├── assets/
│   ├── audio/        # File âm thanh
│   ├── graphics/     # Hình ảnh game
│   └── fonts/        # Font chữ
├── src/
│   ├── main.cpp      # Chương trình chính
│   ├── game.h        # Header file
│   ├── init.cpp      # Khởi tạo game
│   ├── update.cpp    # Logic game
│   └── render.cpp    # Hiển thị game
└── README.md         # Hướng dẫn


