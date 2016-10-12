#include "Logging.h"
#include "BitmapRenderer.h"
#include "graphics/Texture.h"
#include "graphics/ShaderProgram.h"
#include "Color.h"

// TODO: better way
#define RECTANGLE3(x,y,w,h) {x,y,0,x+(w),y,0,x,y+(h),0,x+(w),y+(h),0}
#define RGBA4(r,g,b,a) {r,g,b,a,r,g,b,a,r,g,b,a,r,g,b,a}
#define SETFIELDIN4x4(a,i,v) {a[(i)] = a[(i)+4] = a[(i)+8] = a[(i)+12] = v;}
#define RECTANGLE_TEXCOORD(w,h) {0.0f,h,w,h,0.0f,0.0f,w,0.0f}

BitmapRenderer::BitmapRenderer(const ShaderProgram &shaderProgram) : mShaderProgram(shaderProgram) {

}

void BitmapRenderer::renderTexture(const Texture &texture, float left, float bottom, float width, float height) const {
    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    if (width == SIZE_AUTO && height == SIZE_AUTO) {
        Log::error("renderTexture with both width and height set to auto is currently not supported");
        return;
    }else if (width == SIZE_AUTO) {
        width = height * texture.aspectRatio;
    }else if (height == SIZE_AUTO) {
        height = width / texture.aspectRatio;
    }


    GLfloat position[] = RECTANGLE3(left, bottom, width, height);
    GLfloat textureCoordinates[] = RECTANGLE_TEXCOORD(1.0f, 1.0f);

    mShaderProgram.bind();
    mShaderProgram.setPosition(position);
    mShaderProgram.setDefaultColorForRectangle();
    mShaderProgram.setTextureCoordinates(textureCoordinates);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void BitmapRenderer::renderSolidRectangle(const Vec2 &bottomLeft, const Vec2 &size, const Color &color) const {
    GLfloat positionData[] = RECTANGLE3(bottomLeft.x(), bottomLeft.y(), size.x(), size.y());
    GLfloat colorData[] = RGBA4(color.red(), color.green(), color.blue(), color.alpha());

    mShaderProgram.bind();
    mShaderProgram.setPosition(positionData);
    mShaderProgram.setColor(colorData);
    mShaderProgram.setUniform("noTexture", 1);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // this is considered the "default" state
    mShaderProgram.setUniform("noTexture", 0);
}
