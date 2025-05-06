// Assets.cpp
#include "Assets.h"

void Assets::loadImage(const std::string& name, const std::string& path) {
    images[name] = path;
}

void Assets::loadSound(const std::string& name, const std::string& path) {
    sounds[name] = path;
}

std::string Assets::getImage(const std::string& name) const {
    auto it = images.find(name);
    return it != images.end() ? it->second : "";
}

std::string Assets::getSound(const std::string& name) const {
    auto it = sounds.find(name);
    return it != sounds.end() ? it->second : "";
}

