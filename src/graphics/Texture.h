#pragma once

#include "platform/graphics.h"

class Texture {
public:

    enum RepeatTexture {
        Repeat, DoNotRepeat
    };


    Texture() { }
    Texture(GLuint _id, int w, int h, float _scaling = 1.0f) : id(_id), width(w), height(h), scaling(_scaling) { aspectRatio = (float) w / h; }
    Texture(const std::string &filename, float scaling = 1.0f, RepeatTexture hRepeat = DoNotRepeat, RepeatTexture vRepeat = DoNotRepeat);
    ~Texture();

    //disable copying, a Texture object should be an exclusive owner of the OpenGL texture
    Texture(const Texture &other) = delete;
    Texture &operator=(const Texture &other) = delete;

    GLuint id = 0;
    int width = 0, height = 0;
    float scaling = 1.0f;
    float aspectRatio = 1.0f;

    void load(const char *filename, float scaling = 1.0f, RepeatTexture hRepeat = DoNotRepeat, RepeatTexture vRepeat = DoNotRepeat);
    void fromGLTexture(GLuint _id, int w, int h, float _scaling = 1.0f);
    void unload();
    void bind() const;

    bool isLoaded() const {
        return (id != 0);
    }
};
