// Assets.h
#ifndef ASSETS_H
#define ASSETS_H

#include <string>
#include <unordered_map>
#include <iostream>


class Assets {
private:
    std::unordered_map<std::string, std::string> images;
    std::unordered_map<std::string, std::string> sounds;

public:
    void loadImage(const std::string& name, const std::string& path);
    void loadSound(const std::string& name, const std::string& path);
    std::string getImage(const std::string& name) const;
    std::string getSound(const std::string& name) const;

    static void Init(SDL_Renderer* renderer) {
        // Add initialization logic here, e.g., loading textures or assets
        std::cout << "Assets initialized with renderer." << std::endl;
    }
};

#endif // ASSETS_H