#include "Logging.h"
#include "Theme.h"

Theme::Theme() {

}

Theme::~Theme() {

}

const Texture &Theme::getTexture(const std::string &key) const {
    if (mTextures.find(key) == mTextures.end()) {
        Log::error("No such texture in theme: %s", key.c_str());
    }
    return *mTextures.find(key)->second;
}

void Theme::tempPutTexture(const std::string &key, Texture::RepeatTexture repeat, const std::string &baseFileName) {
    std::string name = baseFileName;
    if (baseFileName.empty()) {
        name = key;
    }
    mTextures[key] = new Texture(std::string("pictures/game/") + name + ".png", 1.0f, repeat, repeat);
}
