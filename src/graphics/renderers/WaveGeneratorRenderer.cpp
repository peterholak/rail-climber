#include "WaveGeneratorRenderer.h"
#include "platform/graphics.h"
#include "graphics/ShaderProgram.h"
#include "game_logic/WaveGenerator.h"
#include "graphics/Theme.h"
#include <math.h>

#define RECTANGLE3(x,y,w,h) {x,y,0,x+(w),y,0,x,y+(h),0,x+(w),y+(h),0}
#define RGBA4(r,g,b,a) {r,g,b,a,r,g,b,a,r,g,b,a,r,g,b,a}
#define SETFIELDIN4x4(a,i,v) {a[(i)] = a[(i)+4] = a[(i)+8] = a[(i)+12] = v;}
#define RECTANGLE_TEXCOORD(w,h) {0.0f,0.0f,w,0.0f,0.0f,h,w,h}

void WaveGeneratorRenderer::render(const WaveGenerator *generator, const Theme *theme, const Vec2 &scroll) {
    float r = WaveGenerator::radius;
    GLfloat coords[] = RECTANGLE3(generator->x - r, generator->y - r, 2 * r, 2 * r);
    GLfloat texcoord[] = RECTANGLE_TEXCOORD(1.0f, 1.0f);
    GLfloat color[] = RGBA4(1, 1, 1, 1);
    mDefaultProgram.bind();
    mDefaultProgram.setPosition(coords);
    mDefaultProgram.setTextureCoordinates(texcoord);
    mDefaultProgram.setColor(color);

    if (generator->active) {
        theme->getTexture("wave-generator-active").bind();
    }else{
        theme->getTexture("wave-generator-inactive").bind();
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    if (generator->active) {
        GLfloat cCoords[66];
        GLfloat cColor[88];
        r = generator->currentRadius();
        for (int i = 1; i < 21; i++) {
            cCoords[i * 3] = generator->x + mCos[i - 1] * r;
            cCoords[i * 3 + 1] = generator->y + mSin[i - 1] * r;
            cCoords[i * 3 + 2] = 0;
            cColor[i * 4] = cColor[i * 4 + 1] = std::min(0.5f, (float) (i % 5 < 3));
            cColor[i * 4 + 2] = std::min(0.4f, (float) (i % 5 < 3));
            cColor[i * 4 + 3] = 0.5;
        }
        cCoords[0] = generator->x;
        cCoords[1] = generator->y;
        cCoords[2] = 0;
        cColor[0] = cColor[1] = cColor[2] = 1;
        cColor[3] = 0;
        cCoords[63] = cCoords[3];
        cCoords[64] = cCoords[4];
        cCoords[65] = 0;
        cColor[84] = cColor[4];
        cColor[85] = cColor[5];
        cColor[86] = cColor[6];
        cColor[87] = cColor[7];
        mDefaultProgram.resetDefaultAttributes();
        mDefaultProgram.setPosition(cCoords);
        mDefaultProgram.setColor(cColor);
        mDefaultProgram.setUniform("noTexture", 1);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
        mDefaultProgram.setUniform("noTexture", 0);
    }
}

WaveGeneratorRenderer::WaveGeneratorRenderer(const ShaderProgram &defaultProgram) : mDefaultProgram(defaultProgram) {
    mCos = new float[20];
    mSin = new float[20];
    for (int i = 0; i < 20; i++) {
        mCos[i] = cos(i * 18 * (M_PI / 180));
        mSin[i] = sin(i * 18 * (M_PI / 180));
    }
}
