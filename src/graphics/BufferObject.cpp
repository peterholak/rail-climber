#include "BufferObject.h"
#include "Logging.h"

BufferObject::BufferObject(const GLfloat *data, int count, GLenum usage) {
    BufferObject();
    create(data, count, usage);
}

BufferObject::~BufferObject() {
    unload();
}

void BufferObject::create(const GLfloat *data, int count, GLenum usage) {
    if (mVBO != 0) {
        Log::warn("BufferObject::create(): buffer already exists");
        unload();
    }

    unbind();
    glGenBuffers(1, &mVBO);
    int error = glGetError();
    if (error != GL_NO_ERROR) {
        Log::error("Error while creating a new buffer [%d]", error);
    }

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    error = glGetError();
    if (error != GL_NO_ERROR) {
        Log::error("Error while binding a newly created buffer [%d]", error);
    }

    glBufferData(GL_ARRAY_BUFFER, count * sizeof (GLfloat), data, usage);
    error = glGetError();
    if (error != GL_NO_ERROR) {
        Log::error("Error while putting data into a buffer [%d]", error);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BufferObject::unload() {
    if (mVBO) {
        glDeleteBuffers(1, &mVBO);
    }
    mVBO = 0;
}

void BufferObject::bind() const {
    if (mVBO == 0) {
        Log::warn("Trying to bind an invalid buffer object");
    }
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        // TODO: ...
        #ifdef __glew_h__
        Log::error("Error while trying to bind buffer: [%d] %s", (int)error, glewGetErrorString(error));
        #else
        Log::error("Error while trying to bind buffer: [%d]", (int)error);
        #endif
    }
}

void BufferObject::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
