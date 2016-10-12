#include "ShaderProgram.h"
#include "Color.h"
#include "platform/core.h"
#include "platform/graphics.h"
#include "Logging.h"

using namespace std;

const float ShaderProgram::defaultColor[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

ShaderProgram::ShaderProgram(const char *vertFilename, const char *fragFilename) {
    mProgram = 0;
    load(vertFilename, fragFilename);
}

ShaderProgram::~ShaderProgram() {
    unload();
}

void ShaderProgram::bind() const {
    glUseProgram(mProgram);
}

void ShaderProgram::unload() {
    if (mProgram) {
        glDeleteProgram(mProgram);
    }
    mProgram = 0;
    mAttPosition = 0;
    mAttColor = 0;
    mAttTexCoords = 0;
    mUniformLocations.clear();
}

GLuint ShaderProgram::loadShaderFromFile(const char *filename, GLenum type) {
    Log::info("Loading shader from %s", filename);

    //load the shader source from file
    int sourceLength;
    char *shaderSource = RailClimberPlatform::loadFile(filename, &sourceLength);
    GLuint shader = loadShader(shaderSource, sourceLength, type);
    delete[] shaderSource;

    return shader;
}

GLuint ShaderProgram::loadShader(const char *source, GLint sourceLength, GLenum type) {
    GLuint shader;
    GLint compiled;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, &sourceLength);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        Log::error("Shader failed to compile");
    }
    char shaderLog[10240];
    glGetShaderInfoLog(shader, 10240, nullptr, shaderLog);
    Log::debug("GL: %s", shaderLog);

    if (!compiled) {
        return 0;
    }

    return shader;
}

void ShaderProgram::load(const char *vertFilename, const char *fragFilename) {
    GLuint vertexShader = loadShaderFromFile(vertFilename, GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShaderFromFile(fragFilename, GL_FRAGMENT_SHADER);

    if (!vertexShader || !fragmentShader) {
        Log::error("Failed to load shaders");
        mProgram = 0;
        return;
    }

    mProgram = glCreateProgram();
    glAttachShader(mProgram, vertexShader);
    glAttachShader(mProgram, fragmentShader);
    glLinkProgram(mProgram);

    glDetachShader(mProgram, vertexShader);
    glDetachShader(mProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    mAttPosition = glGetAttribLocation(mProgram, "a_position");
    mAttColor = glGetAttribLocation(mProgram, "a_color");
    mAttTexCoords = glGetAttribLocation(mProgram, "a_texcoord");

    mProgName = fragFilename;
}

void ShaderProgram::resetDefaultAttributes() const {
    glDisableVertexAttribArray(mAttPosition);
    glDisableVertexAttribArray(mAttColor);
    glDisableVertexAttribArray(mAttTexCoords);
}

void ShaderProgram::setPosition(const BufferObject &vbo) const {
    glEnableVertexAttribArray(mAttPosition);
    vbo.bind();
    glVertexAttribPointer(mAttPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    if (glGetError() != GL_NO_ERROR) {
        Log::error("Error while trying to glVertexAttribPointer");
    }
}

void ShaderProgram::setPosition(const GLfloat *data) const {
    BufferObject::unbind();
    glEnableVertexAttribArray(mAttPosition);
    glVertexAttribPointer(mAttPosition, 3, GL_FLOAT, GL_FALSE, 0, data);
}

void ShaderProgram::setColor(const BufferObject &vbo) const {
    glEnableVertexAttribArray(mAttColor);
    vbo.bind();
    glVertexAttribPointer(mAttColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void ShaderProgram::setColor(const GLfloat *data) const {
    BufferObject::unbind();
    glEnableVertexAttribArray(mAttColor);
    glVertexAttribPointer(mAttColor, 4, GL_FLOAT, GL_FALSE, 0, data);
}

void ShaderProgram::setTextureCoordinates(const BufferObject &vbo) const {
    glEnableVertexAttribArray(mAttTexCoords);
    vbo.bind();
    glVertexAttribPointer(mAttTexCoords, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void ShaderProgram::setTextureCoordinates(const GLfloat data[]) const {
    BufferObject::unbind();
    glEnableVertexAttribArray(mAttTexCoords);
    glVertexAttribPointer(mAttTexCoords, 2, GL_FLOAT, GL_FALSE, 0, data);
}

GLint ShaderProgram::getUniformLocation(const char *uniName) const {
    map<string, GLint>::iterator it = mUniformLocations.find(uniName);
    GLint loc = 0;
    if (it == mUniformLocations.end()) {
        loc = glGetUniformLocation(mProgram, uniName);
    } else {
        loc = it->second;
    }
    if (loc < 0) {
        Log::error("No such uniform: '%s', program '%s'", uniName, mProgName.c_str());
    }
    return loc;
}

// TODO: check if bound
// TODO: more debug-only-compiled checks (like assertions, is this program i use? etc.)

void ShaderProgram::setUniform(const char *name, float value) const {
    glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const char *name, int value) const {
    glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const char *name, float val1, float val2) const {
    glUniform2f(getUniformLocation(name), val1, val2);
}

void ShaderProgram::setUniform(const char *name, float val1, float val2, float val3) const {
    glUniform3f(getUniformLocation(name), val1, val2, val3);
}

void ShaderProgram::setUniform(const char *name, float val1, float val2, float val3, float val4) const {
    glUniform4f(getUniformLocation(name), val1, val2, val3, val4);
}

void ShaderProgram::setUniformMatrix4(const char *name, float *vals) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, vals);
}

void ShaderProgram::setDefaultColorForRectangle() const {
    setColor(defaultColor);
}

void ShaderProgram::setColor(const Color &color) const {
    // TODO:
    throw "Not implemented yet.";
}
