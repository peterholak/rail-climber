#pragma once

#include "graphics/Texture.h"

#include <map>
#include <string>

class Theme {
public:
    Theme();
    ~Theme();

    const Texture &getTexture(const std::string &key) const;
    void tempPutTexture(const std::string &key, Texture::RepeatTexture repeat = Texture::DoNotRepeat, const std::string &baseFileName = std::string());

private:
    std::map<std::string, Texture*> mTextures;
};
