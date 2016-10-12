#pragma once

#include "platform/graphics.h"

class BufferObject {
public:
    BufferObject() : mVBO(0) { }
    BufferObject(const GLfloat *data, int count, GLenum usage = GL_STATIC_DRAW);
    ~BufferObject();

    void create(const GLfloat *data, int count, GLenum usage = GL_STATIC_DRAW);
    void unload();
    void bind() const;

    static void unbind();

private:
    GLuint mVBO;

    //disable copying
    BufferObject(const BufferObject &other) : mVBO(0) { }
    BufferObject &operator=(const BufferObject &other) {
        return *this;
    }
};
