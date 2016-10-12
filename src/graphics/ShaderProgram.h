#pragma once

#include "platform/graphics.h"
#include "graphics/BufferObject.h"
#include <map>
#include <string>

#define VERTEX_SHADER "shaders/main-v.glsl"

class Color;

class ShaderProgram {
public:

    ShaderProgram() : mProgram(0), mAttPosition(0), mAttColor(0), mAttTexCoords(0) { }
    ShaderProgram(const char *vertFilename, const char *fragFilename);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram &other) = delete;
    ShaderProgram &operator=(const ShaderProgram &other) = delete;

    void bind() const;
    void load(const char *vertFilename, const char *fragFilename);
    void unload();

    bool isLoaded() const {
        return (mProgram != 0);
    }

    void setUniform(const char *name, int value) const;
    void setUniform(const char *name, float value) const;
    void setUniform(const char *name, float val1, float val2) const;
    void setUniform(const char *name, float val1, float val2, float val3) const;
    void setUniform(const char *name, float val1, float val2, float val3, float val4) const;
    void setUniformMatrix4(const char *name, float *vals) const;

    //default attributes used by rail climber
    void resetDefaultAttributes() const;
    void setPosition(const BufferObject &vbo) const;
    void setPosition(const GLfloat *data) const;
    void setColor(const BufferObject &vbo) const;
    void setColor(const GLfloat *data) const;
    void setColor(const Color &color) const;
    void setDefaultColorForRectangle() const;
    void setTextureCoordinates(const BufferObject &vbo) const;
    void setTextureCoordinates(const GLfloat data[]) const;

private:
    GLuint loadShaderFromFile(const char *filename, GLenum type);
    GLuint loadShader(const char *source, GLint sourceLength, GLenum type);
    GLuint mProgram;

    GLint mAttPosition;
    GLint mAttColor;
    GLint mAttTexCoords;

    // this is basically just a cache to avoid calling glGetUniformLocation all the time
    mutable std::map<std::string, GLint> mUniformLocations;
    std::string mProgName;

    GLint getUniformLocation(const char *uniName) const;
    void downgradeTo120();

    static const float defaultColor[];
};
