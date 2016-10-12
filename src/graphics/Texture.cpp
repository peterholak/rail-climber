#include "Texture.h"
#include "platform/graphics.h"
#include "Logging.h"

Texture::Texture(const std::string &filename, float scaling, RepeatTexture hRepeat, RepeatTexture vRepeat) {
    load(filename.c_str(), scaling, hRepeat, vRepeat);
}

Texture::~Texture() {
    //	Log::debug("Texture object destructor, id %d", id);
    unload();
}

void Texture::unload() {
    if (id != 0) {
        glDeleteTextures(1, &id);
    }
    id = 0;
}

void Texture::bind() const {
    if (id == 0) {
        Log::warn("Binding an unloaded texture");
    }
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::load(const char *filename, float scaling, RepeatTexture hRepeat, RepeatTexture vRepeat) {
    if (id != 0) {
        Log::warn("Loading a texture '%s' into an object that already had a texture (unloading the old one)", filename);
        unload();
    }

    int texW, texH;
    GLuint texId = RailClimberPlatform::loadTexture(filename, &texW, &texH, hRepeat == Repeat, vRepeat == Repeat);

    if (!texId) {
        Log::error("Can't load texture from '%s'", filename);
    }

    this->id = texId;
    this->width = texW;
    this->height = texH;
    this->aspectRatio = (float) width / height;
    this->scaling = scaling;
}

void Texture::fromGLTexture(GLuint _id, int w, int h, float _scaling) {
    if (id != 0) {
        Log::warn("Loading a GL texture into an object that already had a texture (unloading the old one)");
        unload();
    }
    id = _id;
    width = w;
    height = h;
    scaling = _scaling;
    aspectRatio = (float) width / height;

    if (id == 0) {
        Log::warn("Texture object set to non-existing OpenGL texture (id 0)");
    }
}
