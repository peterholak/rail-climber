#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Painter.h"

#include "game_logic/World.h"
#include "game_logic/GameScene.h"
#include "game_logic/WaveGenerator.h"
#include "graphics/IFadeCallback.h"
#include "Settings.h"
#include "Translation.h"
#include "game_logic/Enemy.h"
#include "game_logic/UnitConverter.h"
//#include "ForceField.h"
#include "Logging.h"
#include "ui/MenuScreen.h"
#include "ui/Label.h"
#include <math.h>
#include <vector>
#include <list>
#include <stdio.h>
#include <iostream>

using namespace std;

Painter::Painter(const shared_ptr<UnitConverter> &unitConverter) :
    scrollX(0), scrollY(0), aaOverdraw(true), mUnitConverter(unitConverter)
{
    mDefaultFBO = mSnapshotFBO = 0;

    mScreenW = mScreenH = mRatio = mRatio10 = mScaleRatio = 0;
    mGameSnapshot = mRequestedSnapshot = false;

    hazardTime = 0;
    hazardFrame = hazardFrameTicks = 0;
    showVolume = false;
    volume = 0;
    aaOverdraw = false;

    mFading = false;
    mFadeDuration = 0;
    mFadeDirection = 0;
    mFadeTime = 0;
    mFadeWhite = false;
    mFadeTextWidth = 1;
    mFadeTextHeight = 1;
    mFadeTextDuration = 0;
    mFadeTextPhase = 0;
    mFadeIsText = false;
    mFadeEasing = Linear;
    mFadeEndDuration = 0;
    mFadeCallback = nullptr;

    activeBkg = 0;
    activeTheme = 0;

    quitting = false;
}

Painter::~Painter() {

}

void Painter::initialize(float screenW, float screenH, float scaleRatio) {
    mScreenW = screenW;
    mScreenH = screenH;
    mRatio = screenW / screenH;
    mRatio10 = mRatio * 10.0f;
    mScaleRatio = scaleRatio;
    mGameSnapshot = false;
    mRequestedSnapshot = false;

    hazardTime = 0;
    hazardFrame = hazardFrameTicks = 0;

    showVolume = false;

    if (mScreenW / scaleRatio == 320 && mScreenH / scaleRatio == 480) {
        mResolutionSuffix = "hvga";
    } else {
        mResolutionSuffix = "wvga";
    }

//    mainFont.load("fonts/Exo-Light.ttf", mScreenH / 20);
//    smallFont.load("fonts/Exo-Light.ttf", mScreenH / 40);

    for (int i = 0; i < 20; i++) {
        float pi = 3.141592654f;
        mCos[i] = cos(i * 18 * (pi / 180));
        mSin[i] = sin(i * 18 * (pi / 180));
    }

    Log::debug("Painter::Initialize");
}

void Painter::createGraphics() {
    mAutoDelTextures.clear();
    mTextLabelQueue.clear();
    mWidgetQueue.clear();

    //prepareSnapshotFramebuffer();

    loadAutoTexture(texBackground[0], "tex/day/background.png", true, 1.0f, true);
    loadAutoTexture(texBackground[1], "tex/night/background.png", true, 1.0f, true);
    loadAutoTexture(texBackground[2], "tex/sunset/background.png", true, 1.0f, true);
    loadAutoTexture(texRail[0], "tex/day/rail.png", true);
    loadAutoTexture(texRail[1], "tex/night/rail.png", true);
    loadAutoTexture(texRail[2], "tex/sunset/rail.png", true);
    loadAutoTexture(texRailDisabled, "tex/rail_disabled.png", true);
    loadAutoTexture(texBall, "tex/ball.png", false);
    loadAutoTexture(texBallGreyed, "tex/ball_greyed.png", false);
    loadAutoTexture(texGlare, "tex/glare.png", false);
    loadAutoTexture(texFloor[0], "tex/summer/floor.png", true, 0.8f);
    loadAutoTexture(texFloor[1], "tex/snow/floor.png", true, 0.8f);
    loadAutoTexture(texWaveGenA, "tex/wavegen_active.png", false);
    loadAutoTexture(texWaveGenI, "tex/wavegen_inactive.png", false);
    loadAutoTexture(texGBoost, "tex/gboost.png", false);
    for (unsigned int i = 0; i < NOISE_FRAMES; i++) {
        char noiseFN[128];
        snprintf(noiseFN, 128, "tex/noise/noise%03d.png", i + 1);
        loadAutoTexture(texNoise[i], noiseFN, true, 0.6f, false);
    }
    loadAutoTexture(texPause, "tex/pause.png", false);
    loadAutoTexture(texMenuBkg, (string("tex/rc_menubkg_") + mResolutionSuffix + ".png").c_str(), false);
    loadAutoTexture(texGrassStart[0], "tex/summer/grass_start.png", false);
    loadAutoTexture(texGrassStart[1], "tex/snow/grass_start.png", false);
    loadAutoTexture(texGrassMid[0], "tex/summer/grass_mid.png", true, 1.0f, true);
    loadAutoTexture(texGrassMid[1], "tex/snow/grass_mid.png", true, 1.0f, true);
    loadAutoTexture(texGrassEnd[0], "tex/summer/grass_end.png", false);
    loadAutoTexture(texGrassEnd[1], "tex/snow/grass_end.png", false);
    loadAutoTexture(texBullet[0], "tex/summer/bullet.png", false);
    loadAutoTexture(texBullet[1], "tex/snow/bullet.png", false);
    loadAutoTexture(texLevel, "tex/level_enabled.png", false);
    loadAutoTexture(texLevelDown, "tex/level_down.png", false);
    loadAutoTexture(texLevelDisabled, "tex/level_disabled.png", false);
    loadAutoTexture(texLevelPassed, "tex/level_passed.png", false);
    loadAutoTexture(texLevelPassedDown, "tex/level_passed_down.png", false);
    loadAutoTexture(texFingerUp, "tex/tips/finger.png", false);
    loadAutoTexture(texFingerDown, "tex/tips/finger_down.png", false);
    loadAutoTexture(texFinish, "tex/finish.png", true);
    loadAutoTexture(texOn, "tex/on.png", false);
    loadAutoTexture(texOnDown, "tex/on_down.png", false);
    loadAutoTexture(texOff, "tex/off.png", false);
    loadAutoTexture(texOffDown, "tex/off_down.png", false);
    loadAutoTexture(texMenuOverlay, "tex/menuoverlay.png", false);
    loadAutoTexture(texMedal[0], "tex/medal_gold.png", false);
    loadAutoTexture(texMedal[1], "tex/medal_silver.png", false);
    loadAutoTexture(texMedal[2], "tex/medal_bronze.png", false);
    loadAutoTexture(texMedal[3], _("tex/medal_no.png").c_str(), false);
    loadAutoTexture(texTinyMedal[0], "tex/tinymedal_gold.png", false);
    loadAutoTexture(texTinyMedal[1], "tex/tinymedal_silver.png", false);
    loadAutoTexture(texTinyMedal[2], "tex/tinymedal_bronze.png", false);
    loadAutoTexture(texBack, "tex/back.png", false);
    loadAutoTexture(texBackDown, "tex/back_down.png", false);
    for (int i = 0; i < 4; i++) {
        char enemyStr[128];
        snprintf(enemyStr, 128, "tex/enemy_%d_o.png", i + 1);
        loadAutoTexture(texEnemy[1][i], enemyStr, false);
        snprintf(enemyStr, 128, "tex/enemy_%d_c.png", i + 1);
        loadAutoTexture(texEnemy[0][i], enemyStr, false);
        snprintf(enemyStr, 128, "tex/enemy_f_%d_o.png", i + 1);
        loadAutoTexture(texEnemyFast[1][i], enemyStr, false);
        snprintf(enemyStr, 128, "tex/enemy_f_%d_c.png", i + 1);
        loadAutoTexture(texEnemyFast[0][i], enemyStr, false);
    }
    loadAutoTexture(texBkgTop[0], "tex/day/background_top.png", true);
    loadAutoTexture(texBkgTop[1], "tex/night/background_top.png", true);
    loadAutoTexture(texBkgTop[2], "tex/sunset/background_top.png", true);

//    mainFont.createTextLabel(texFPS, "FPS: 0");
//    mainFont.createTextLabel(texTime, "0:00");

    prgDefault.load("shaders/main.vert", "shaders/main.frag");
    prgHazard.load("shaders/main.vert", "shaders/smoke.frag");
    prgDeadlyRail.load("shaders/main.vert", "shaders/deadly-rail.frag");

    prgDefault.bind();
    prgDefault.setUniform("u_scroll", 0.0f, 0.0f);

    glViewport(0, 0, mScreenW, mScreenH);

    glm::mat4 camera = glm::ortho(0.0f, mRatio * 10.0f * mScaleRatio, 0.0f, 10.0f * mScaleRatio, -1.0f, 1.0f);
    float *cameraVal = glm::value_ptr<float>(camera);

    prgDefault.setUniformMatrix4("projection", cameraVal);
    prgDefault.setUniform("noTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    prgDefault.setUniform("texture", 0);

    prgHazard.bind();
    prgHazard.setUniformMatrix4("projection", cameraVal);
    prgHazard.setUniform("texture", 0);

    prgDeadlyRail.bind();
    prgDeadlyRail.setUniformMatrix4("projection", cameraVal);
    prgDeadlyRail.setUniform("texture", 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.59f, 0.72f, 0.9f, 1.0f);
}

void Painter::destroyGraphics() {
    for (list<Texture*>::iterator it = mAutoDelTextures.begin(); it != mAutoDelTextures.end(); ++it) {
        (*it)->unload();
    }
    mAutoDelTextures.clear();
    for (list<TextLabelData*>::iterator it = mTextLabelQueue.begin(); it != mTextLabelQueue.end(); ++it) {
        delete *it;
    }
    mTextLabelQueue.clear();
    for (list<WidgetData*>::iterator it = mWidgetQueue.begin(); it != mWidgetQueue.end(); ++it) {
        delete *it;
    }

    mWidgetQueue.clear();
    texTime.unload();
    prgDefault.unload();
    prgHazard.unload();
    prgDeadlyRail.unload();
}

void Painter::cleanup() {
//    mainFont.unload();
////    smallFont.unload();
    destroyGraphics();
}

void Painter::prepareSnapshotFramebuffer() {
    GLint signedFBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &signedFBO);
    mDefaultFBO = signedFBO;
    glGenFramebuffers(1, &mSnapshotFBO);
    glGenTextures(1, &texGameSnapshot.id);
    texGameSnapshot.width = mScreenW;
    texGameSnapshot.height = mScreenH;
    glBindTexture(GL_TEXTURE_2D, texGameSnapshot.id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    char *zero = new char[(int) mScreenW * (int) mScreenH * 4];
    memset(zero, 100, mScreenW * mScreenH * 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mScreenW, mScreenH, 0, GL_RGBA, GL_UNSIGNED_BYTE, zero);
    delete[] zero;
}

GLuint Painter::loadShader(GLenum type, const char *filename) {
    GLuint shader;
    GLint compiled;

    Log::info("Loading shader from %s", filename);

    //load the shader source from file
    int len;
    char *str = RailClimberPlatform::loadFile(filename, &len);

    //compile the shader
    shader = glCreateShader(type);
    const char *src = str;
    glShaderSource(shader, 1, &src, &len);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    //if (!compiled) {
    char shaderLog[10240];
    glGetShaderInfoLog(shader, 10240, nullptr, shaderLog);
    Log::debug("GL: %s", shaderLog);

    delete[] str;
    if (!compiled) {
        return 0;
    }

    return shader;
}

GLuint Painter::loadProgram(const char *vertexFile, const char *fragmentFile) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

    if (!vertexShader || !fragmentShader) {
        Log::error("Failed to load shaders");
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glUseProgram(program);

    return program;
}

Texture *Painter::loadTexture(const char *filename, bool repeat) {
    int texW, texH;
    GLuint texId = RailClimberPlatform::loadTexture(filename, &texW, &texH, repeat, repeat);
    if (!texId) {
        Log::error("Can't load texture from '%s'", filename);
    }
    return new Texture(texId, texW, texH);
}

void Painter::loadAutoTexture(Texture &tex, const char *filename, bool repeat, float scaling, bool cancelVRepeat, GLenum sourceFormat) {
    int texW, texH;
    GLuint texId = RailClimberPlatform::loadTexture(filename, &texW, &texH, repeat, repeat && (!cancelVRepeat));
    if (!texId) {
        Log::error("Can't load texture from '%s'", filename);
    }
    tex.load(filename, scaling, repeat ? Texture::Repeat : Texture::DoNotRepeat, (repeat && (!cancelVRepeat)) ? Texture::Repeat : Texture::DoNotRepeat);
    mAutoDelTextures.push_back(&tex);
}

Texture *Painter::createTextLabel(RCLFontType font, const char *text, int color, bool multi, float multiW) {
    int texW, texH;
    GLuint texId;
    multiW = (multiW * mScreenH) / 10;
    if (!multi) {
        texId = RailClimberPlatform::textureFromText(font, color, text, &texW, &texH);
    } else {
        texId = RailClimberPlatform::multiLineText(font, color, text, multiW, &texH, RailClimberPlatform::Center);
        texW = multiW;
        texH *= mScaleRatio;
    }

    return new Texture(texId, texW, texH);
}

void Painter::enqueueCreateTextLabel(Texture *target, Font *font, const char *text, int color, float *ptrW, float *ptrH, Label *lbl, bool multi, float multiW, Font::TextJustify justify) {
    TextLabelData *tld = new TextLabelData();
    tld->target = target;
    tld->font = font;
    tld->text = text;
    tld->color = color;
    tld->ptrW = ptrW;
    tld->ptrH = ptrH;
    tld->lbl = lbl;
    tld->multi = multi;
    tld->multiW = multiW;
    tld->justify = justify;
    mTextLabelQueue.push_back(tld);
}

void Painter::enqueueWidgetGLThread(Widget *w, void *data) {
    WidgetData *wd = new WidgetData();
    wd->w = w;
    wd->data = data;
    mWidgetQueue.push_back(wd);
}

void Painter::drawRail(Rail *r) {
    GLfloat *texcoord;
    GLfloat *color;
    GLfloat *rail;

    const vector<Vec2> &points = r->getPoints();
    int nSegments = points.size() - 1;
    GLfloat thick = 0.055f;

    if (r->boxCache[2] == nullptr) {
        r->boxCache[2] = new float[nSegments * 12];
        texcoord = r->boxCache[2];
        for (int i = 0; i < nSegments; i++) {
            texcoord[i * 12] = texcoord[i * 12 + 1] = texcoord[i * 12 + 3] = texcoord[i * 12 + 4] = texcoord[i * 12 + 7] = texcoord[i * 12 + 8] = 0.0f;
            texcoord[i * 12 + 2] = texcoord[i * 12 + 6] = texcoord[i * 12 + 10] = 2.0f;
            texcoord[i * 12 + 5] = texcoord[i * 12 + 9] = texcoord[i * 12 + 11] = 1.0f;
        }
    } else {
        texcoord = r->boxCache[2];
    }

    //cache the "box around line" coordinates for the rail
    //TODO: make a vbo buffer to get even more speed
    if (r->boxCache[0] == nullptr) {
        r->boxCache[0] = new float[nSegments * 18];
        rail = r->boxCache[0];
        for (int i = 0; i < nSegments; i++) {
            Vec2 r1, r2, r3, r4;
            float x1 = points[i].x(), y1 = points[i].y();
            float x2 = points[i + 1].x(), y2 = points[i + 1].y();
            float dist = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2))*3; //used to make textures the same size on all rails
            Vec2::boxAroundLine(thick, Vec2(x1, y1), Vec2(x2, y2), r1, r2, r3, r4);
            texcoord[i * 12 + 2] = texcoord[i * 12 + 6] = texcoord[i * 12 + 10] = dist;
            rail[i * 18 + 2] = rail[i * 18 + 5] = rail[i * 18 + 8] = rail[i * 18 + 11] = rail[i * 18 + 14] = rail[i * 18 + 17] = 0.0f;
            rail[i * 18 + 0] = r1.x();
            rail[i * 18 + 1] = r1.y();
            rail[i * 18 + 3] = r2.x();
            rail[i * 18 + 4] = r2.y();
            rail[i * 18 + 6] = r3.x();
            rail[i * 18 + 7] = r3.y();
            rail[i * 18 + 9] = rail[i * 18 + 3];
            rail[i * 18 + 10] = rail[i * 18 + 4];
            rail[i * 18 + 12] = rail[i * 18 + 6];
            rail[i * 18 + 13] = rail[i * 18 + 7];
            rail[i * 18 + 15] = r4.x();
            rail[i * 18 + 16] = r4.y();
        }
    } else {
        rail = r->boxCache[0];
    }

    if (r->boxCache[1] == nullptr) {
        r->boxCache[1] = new float[nSegments * 24];
        color = r->boxCache[1];
        for (int i = 0; i < nSegments * 24; i++) {
            color[i] = 1;
        }
    } else {
        color = r->boxCache[1];
    }

    if (r->deadly) {
        prgDeadlyRail.bind();
        prgDeadlyRail.setPosition(rail);
        prgDeadlyRail.setTextureCoordinates(texcoord);
        prgDeadlyRail.setColor(color);
        prgDeadlyRail.setUniform("u_scroll", scrollX, scrollY);
        prgDeadlyRail.setUniform("drProgress", (r->deadlyStage - 1) * r->deadlyProgress, 0, (r->deadlyStage - 2) * -0.8, (r->disabled ? -0.4 : 0));
    } else {
        prgDefault.bind();
        prgDefault.setPosition(rail);
        prgDefault.setColor(color);
        prgDefault.setTextureCoordinates(texcoord);
    }

    if (r->disabled) {
        texRailDisabled.bind();
    } else {
        texRail[activeBkg].bind();
    }
    glDrawArrays(GL_TRIANGLES, 0, nSegments * 6);
    if (r->disabled) {
        texRail[activeBkg].bind();
    }

    if (aaOverdraw) {
        overdrawRailAA(r);
    }

    prgDefault.bind();
}

void Painter::overdrawRailAA(Rail *r) {
    prgDefault.bind();
    prgDefault.setUniform("noTexture", 1);
    prgDefault.resetDefaultAttributes();

    int nSegments = r->getPoints().size() - 1;
    GLfloat *color;

    int useColor = 1;
    if (r->disabled) {
        useColor = 2;
    }
    if (r->deadly) {
        useColor = 2 + r->deadlyStage;
    }

    if (r->aaOverdrawCache[useColor] == nullptr) {
        r->aaOverdrawCache[useColor] = new GLfloat[32 * nSegments];
        color = r->aaOverdrawCache[useColor];
        GLfloat colorn[] = QUADRGBA(0.16, 0.16, 0.17, 0.45);
        if (r->disabled) {
            SETQUADITEM(colorn, 0, 0.32);
            SETQUADITEM(colorn, 1, 0.34);
            SETQUADITEM(colorn, 2, 0.34);
            SETQUADITEM(colorn, 3, 0.15);
        }
        if (r->deadly) {
            if (r->deadlyStage == 1) {
                SETQUADITEM(colorn, 0, 0.26);
                SETQUADITEM(colorn, 2, 0.98);
            } else if (r->deadlyStage == 2) {
                SETQUADITEM(colorn, 0, 0.4);
                SETQUADITEM(colorn, 2, 0.57);
            } else if (r->deadlyStage == 3) {
                SETQUADITEM(colorn, 0, 0.54);
                SETQUADITEM(colorn, 2, 0.17);
            }
        }

        for (int j = 0; j < nSegments; j++) {
            for (int i = 0; i < 16; i++) {
                color[j * 32 + i] = colorn[i];
                color[j * 32 + i + 16] = colorn[i];
            }
        }
    } else {
        color = r->aaOverdrawCache[useColor];
    }

    GLfloat *coords;
    if (r->aaOverdrawCache[0] == nullptr) {
        r->aaOverdrawCache[0] = new GLfloat[24 * nSegments];
        coords = r->aaOverdrawCache[0];
        float thick = 0.0575f;
        for (int i = 0; i < nSegments; i++) {
            const Vec2 &pos = r->getPoints()[i];
            const Vec2 &nextpos = r->getPoints()[i + 1];
            Vec2 r1, r2, r3, r4;
            Vec2::boxAroundLine(thick, pos, nextpos, r1, r2, r3, r4);
            GLfloat tmp[] = {
                r1.x(), r1.y(), 0,
                r2.x(), r2.y(), 0,
                r4.x(), r4.y(), 0,
                r3.x(), r3.y(), 0,
                r1.x() - 0.01f, r1.y() - 0.004f, 0,
                r2.x() - 0.01f, r2.y() - 0.01f, 0,
                r4.x() + 0.01f, r4.y() + 0.004f, 0,
                r3.x() + 0.01f, r3.y() + 0.01f, 0
            };
            for (int j = 0; j < 24; j++) {
                coords[i * 24 + j] = tmp[j];
            }
        }
    } else {
        coords = r->aaOverdrawCache[0];
    }

    prgDefault.setColor(color);
    prgDefault.setPosition(coords);
    glDrawArrays(GL_LINES, 0, nSegments * 8);

    prgDefault.setUniform("noTexture", 0);
}

//void Painter::drawForceField(ForceField *s) {
/*int nPoints = s->drawPoints.size();
s->fillPointCache(texForceField.scaling);
s->calculatePolygonBuffer(texForceField.scaling, 0.4f); //doesn't do anything if cache is already filled

GLfloat *pts = s->pointCache[StaticObject::POINT_CACHE_COORDINATES];
GLfloat *color = s->pointCache[StaticObject::POINT_CACHE_COLOR];
GLfloat *texcoord = s->pointCache[StaticObject::POINT_CACHE_TEXCOORDS]; //TODO: all 1s on x

glVertexAttribPointer(progDefault_attPosition, 3, GL_FLOAT, GL_FALSE, 0, pts);
glVertexAttribPointer(progDefault_attTexCoord, 2, GL_FLOAT, GL_FALSE, 0, texcoord);
glVertexAttribPointer(progDefault_attColor, 4, GL_FLOAT, GL_FALSE, 0, color);

    glDrawArrays(GL_TRIANGLES, 0, nPoints);

    glVertexAttribPointer(progDefault_attPosition, 3, GL_FLOAT, GL_FALSE, 0, s->bufferCache[StaticObject::BUFFER_CACHE_COORDINATES]);
    glVertexAttribPointer(progDefault_attColor, 4, GL_FLOAT, GL_FALSE, 0, s->bufferCache[StaticObject::BUFFER_CACHE_FADEOUT]);
    glVertexAttribPointer(progDefault_attTexCoord, 2, GL_FLOAT, GL_FALSE, 0, s->bufferCache[StaticObject::BUFFER_CACHE_TEXCOORDS]);

    glDrawArrays(GL_TRIANGLES, 0, s->bufferPointCount);*/
//}

void Painter::drawGrass(const World::Grass *g) {
    GLfloat color[] = QUADRGBA(1, 1, 1, 1);

    prgDefault.bind();
    prgDefault.setPosition(g->pointCache[1]);
    prgDefault.setTextureCoordinates(g->texCache[1]);
    prgDefault.setColor(color);
    glBindTexture(GL_TEXTURE_2D, texGrassMid[activeTheme].id);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    prgDefault.setPosition(g->pointCache[0]);
    prgDefault.setTextureCoordinates(g->texCache[0]);
    glBindTexture(GL_TEXTURE_2D, texGrassStart[activeTheme].id);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    prgDefault.setPosition(g->pointCache[2]);
    prgDefault.setTextureCoordinates(g->texCache[2]);
    glBindTexture(GL_TEXTURE_2D, texGrassEnd[activeTheme].id);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Painter::setScroll(GameScene *g) {
    if (g->inMenu) {
        return;
    }

    World *w = g->getWorld();
    const World::Player &player = w->getPlayer();
    scrollX = player.pos.x()-(5.0 * mRatio);
    scrollY = player.pos.y() - 5.0;
    if (scrollX < 0) {
        scrollX = 0;
    }
    if (scrollY < 0) {
        scrollY = 0;
    }
    if (scrollX > w->levelWidth - mRatio10) {
        scrollX = w->levelWidth - mRatio10;
    }
    g->scrollX = scrollX;
    g->scrollY = scrollY;

    b2AABB drawAABB;
    drawAABB.lowerBound.Set(scrollX - 0.5, scrollY - 0.5);
    drawAABB.upperBound.Set(scrollX + mRatio10 + 0.5, scrollY + 10 + 0.5);
    const b2World *boxWorld = g->getWorld()->getBoxWorld();
    mDrawnAABB.clear();

    if (boxWorld) {
        boxWorld->QueryAABB(this, drawAABB);
    }
}

/**
 * <p>Draws the specified texture to the screen. The texture will be stretched to fit into
 * the [x, y, w, h] rectangle, where x and y are coordinates of the top left point.</p>
 * <p>The width and height can also be 0. In this case, the size will be calculated to match
 * the same number of pixels as the texture has. Note that this will lead to different
 * size (in game coordinates) pictures on different resolutions. If only one of the
 * dimensions is 0, it will be calculated to match the aspect ratio of the texture.</p>
 *
 * @param x horizontal position of the top left point
 * @param y vertical position of the top left point
 * @param adjustScroll If true, the x and y will mean absolute game screen coordinates (ignoring scroll). Otherwise, level coordinates will be used, so scroll will be applied.
 * @param w width of the drawn texture in game coordinates, or 0 to calculate width automatically
 * @param h height of the drawn texture in game coordinates, or 0 to calculate height automatically
 * @param color color that will be mixed with the texture in the fragment shader
 */
void Painter::drawTextureRect(const Texture &tex, float x, float y, bool adjustScroll, float w, float h, GLfloat *color) {
    if (!tex.id) {
        return;
    }

    float rw = w, rh = h;
    if (w == 0 && h == 0) {
        rw = tex.width / (mScreenW / (mRatio * 10.0f));
        rh = tex.height / (mScreenH / 10.0f);
    } else if (w == 0) {
        rw = rh * ((float) tex.width / tex.height);
    } else if (h == 0) {
        rh = rw * ((float) tex.height / tex.width);
    }

    tex.bind();
    drawTextureRect(x, y, adjustScroll, rw, rh, color);
}

/**
 * <p>Draws the currently bound texture to the screen. The texture will be stretched to fit into
 * the a [x, y, w, h] rectangle, where x and y are coordinates of the top left point.</p>
 *
 * @param x horizontal position of the top left point
 * @param y vertical position of the top left point
 * @param adjustScroll If true, the x and y will mean absolute game screen coordinates (ignoring scroll). Otherwise, level coordinates will be used, so scroll will be applied.
 * @param w width of the drawn texture in game coordinates
 * @param h height of the drawn texture in game coordinates
 * @param color color that will be mixed with the texture in the fragment shader
 */
void Painter::drawTextureRect(float x, float y, bool adjustScroll, float w, float h, GLfloat *color) {
    if (adjustScroll) {
        x += scrollX;
        y += scrollY;
    }

    GLfloat coords[] = RECTANGLE(x, y, w, -h);
    GLfloat texcoord[] = RECTTEX(1.0f, 1.0f);
    GLfloat defColor[] = QUADRGBA(1, 1, 1, 1);
    GLfloat *clr = (color ? color : defColor);

    prgDefault.bind();
    prgDefault.setPosition(coords);
    prgDefault.setTextureCoordinates(texcoord);
    prgDefault.setColor(clr);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Painter::drawX(const Vec2 &pos, int color) {
    float x = pos.x() + scrollX;
    float y = pos.y() + scrollY;
    float r = 0.2f, t = 0.025f;

    Vec2 pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8;

    int redI = color & 0xFF, greenI = (color & 0xFF00) >> 8, blueI = (color & 0xFF0000) >> 16;
    float red = redI / 255.0f, green = greenI / 255.0f, blue = blueI / 255.0f;

    Vec2::boxAroundLine(t, Vec2(x - r, y + r), Vec2(x + r, y - r), pt1, pt2, pt3, pt4);
    Vec2::boxAroundLine(t, Vec2(x + r, y + r), Vec2(x - r, y - r), pt5, pt6, pt7, pt8);

    GLfloat coords[] = {
        pt1.x(), pt1.y(), 0, pt2.x(), pt2.y(), 0, pt3.x(), pt3.y(), 0,
        pt2.x(), pt2.y(), 0, pt3.x(), pt3.y(), 0, pt4.x(), pt4.y(), 0,
        pt5.x(), pt5.y(), 0, pt6.x(), pt6.y(), 0, pt7.x(), pt7.y(), 0,
        pt6.x(), pt6.y(), 0, pt7.x(), pt7.y(), 0, pt8.x(), pt8.y(), 0
    };

    GLfloat clr[] = {
        red, green, blue, 1.0f, red, green, blue, 1.0f, red, green, blue, 1.0f,
        red, green, blue, 1.0f, red, green, blue, 1.0f, red, green, blue, 1.0f,
        red, green, blue, 1.0f, red, green, blue, 1.0f, red, green, blue, 1.0f,
        red, green, blue, 1.0f, red, green, blue, 1.0f, red, green, blue, 1.0f
    };

    prgDefault.bind();
    prgDefault.resetDefaultAttributes();
    prgDefault.setPosition(coords);
    prgDefault.setColor(clr);
    prgDefault.setUniform("noTexture", 1);

    glDrawArrays(GL_TRIANGLES, 0, 12);

    prgDefault.setUniform("noTexture", 0);
}

void Painter::fadeIn(int duration, const map<string, string> &callbackMap, FadeEasing easing) {
    mFading = true;
    mFadeDirection = -1;
    mFadeDuration = duration;
    mFadeTime = 0;
    mFadeIsText = false;
    mFadeCallbackMap = callbackMap;
    mFadeEasing = easing;
}

void Painter::fadeIn(int duration, FadeEasing easing) {
    map<string, string> emptyMap;
    fadeIn(duration, emptyMap, easing);
}

void Painter::fadeOut(int duration, const map<string, string> &callbackMap, FadeEasing easing) {
    mFading = true;
    mFadeDirection = 1;
    mFadeDuration = duration;
    mFadeTime = 0;
    mFadeIsText = false;
    mFadeCallbackMap = callbackMap;
    mFadeEasing = easing;
}

void Painter::fadeOut(int duration, FadeEasing easing) {
    map<string, string> emptyMap;
    fadeOut(duration, emptyMap, easing);
}

bool Painter::isFading() {
    return mFading;
}

void Painter::cancelFade() {
    mFading = false;
    if (mFadeCallback) {
        mFadeCallback->fadeCallback(mFadeCallbackMap, true);
    }
}

void Painter::registerFadeCallback(IFadeCallback *callback) {
    mFadeCallback = callback;
}

void Painter::fadeStep() {
    mFadeTime++;
    int duration = mFadeDuration;
    if (mFadeIsText && mFadeTextPhase == 2) {
        duration = mFadeTextDuration;
    }
    if (mFadeIsText && mFadeTextPhase == 3) {
        duration = mFadeEndDuration;
    }
    if (mFadeTime > duration) {
        if (!mFadeIsText) {
            mFading = false;
            if (mFadeCallback) {
                mFadeCallback->fadeCallback(mFadeCallbackMap, false);
            }
        } else {
            //0 = fadeout, 1 = show text, fade in, 2 = show text, no fading, 3 = final fadeout
            mFadeTextPhase++;
            if (mFadeTextPhase == 1) {
                mFadeDirection = -1;
            } else {
                mFadeDirection = 1;
            }
            mFadeTime = 0;
            if (mFadeTextPhase > 3) {
                mFading = false;
                if (mFadeCallback) {
                    mFadeCallback->fadeCallback(mFadeCallbackMap, false);
                }
            }
        }
    }
}

void Painter::fadeOutInTextOut(int fadeDuration, int textDuration, int endDuration, const map<string, string> &callbackMap, const string &fadeText, bool multiLine) {
    mFading = true;
    mFadeDuration = fadeDuration;
    mFadeTextDuration = textDuration;
    mFadeEndDuration = endDuration;
    mFadeIsText = true;
    mFadeTime = 0;
    mFadeCallbackMap = callbackMap;
    mFadeText = fadeText;
    mFadeWhite = false;
    mFadeTextPhase = 0;
    mFadeDirection = 1;

//    enqueueCreateTextLabel(&mFadeTextTexture, &mainFont, fadeText.c_str(), 0xFFFFFFFF, &mFadeTextWidth, &mFadeTextHeight, nullptr, multiLine, mRatio10 - 2.0f, Font::Center);
}

void Painter::drawFadeout() {
    float b = 0, c = 1, t = mFadeTime, d = mFadeDuration;

    GLfloat fadeAlpha = 1;

    if (mFadeEasing == Linear) {
        fadeAlpha = c * t / d + b;
    } else if (mFadeEasing == QuinticIn) {
        t /= d;
        fadeAlpha = c * t * t * t * t * t + b;
    } else if (mFadeEasing == QuadraticIn) {
        t /= d;
        fadeAlpha = c * t * t + b;
    }

    if (mFadeDirection < 0) {
        fadeAlpha = 1.0f - fadeAlpha;
    }

    if (mFadeIsText && mFadeTextPhase == 2) {
        fadeAlpha = 1.0f;
    }

    GLfloat coords[] = {0, 0, 0, 0, 10, 0, mRatio10, 0, 0, mRatio10, 10, 0};

    GLfloat col = mFadeWhite ? 1 : 0;
    GLfloat clr[] = {col, col, col, fadeAlpha, col, col, col, fadeAlpha, col, col, col, fadeAlpha, col, col, col, fadeAlpha};

    prgDefault.bind();
    prgDefault.resetDefaultAttributes();
    prgDefault.setPosition(coords);
    prgDefault.setColor(clr);
    prgDefault.setUniform("noTexture", 1);
    prgDefault.setUniform("u_scroll", 0, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    prgDefault.setUniform("noTexture", 0);
}

void Painter::drawInterface(GameScene *g) {
    const World *w = g->getWorld();

    if (g->dragging) {
        const World::Player &player = const_cast<World*> (w)->getPlayer();
        float x = g->lastX, y = g->lastY;
        Vec2 r1, r2, r3, r4;
        Vec2 force = Vec2(x, y) - (player.pos - g->calcScrollXY());
        float len = force.length();
        if (len > World::maximumForce) {
            force = force.toDistance(World::maximumForce);
        }

        if (force.length() >= World::playerRadius + 0.25f) {
            Vec2 start = player.pos - g->calcScrollXY() + force.toDistance(World::playerRadius + 0.1f);
            Vec2 mid = player.pos - g->calcScrollXY() + force.toDistance(World::playerRadius + 0.25f + len * 0.06f);
            Vec2 end = player.pos - g->calcScrollXY() + force;

            Vec2::boxAroundLine(0.35f, start, end, r1, r2, r3, r4);
            GLfloat coords[] = {
                r1.x(), r1.y(), 0,
                end.x(), end.y(), 0,
                mid.x(), mid.y(), 0,

                r3.x(), r3.y(), 0,
                end.x(), end.y(), 0,
                mid.x(), mid.y(), 0
            };
            bool fire = player.canFire();
            float e = fire ? 1 : 0.5;
            float a = fire ? 0.3 : 0.1;
            GLfloat color[] = {e, e, e, a, e, e, e, a, e, e, e, a, e, e, e, a, e, e, e, a, e, e, e, a};

            prgDefault.bind();
            prgDefault.resetDefaultAttributes();
            prgDefault.setUniform("noTexture", 1);
            prgDefault.setPosition(coords);
            prgDefault.setColor(color);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            prgDefault.setUniform("noTexture", 0);
        }
    }

    GLfloat color[] = QUADRGBA(1, 1, 1, 1);
    if (w->gravityBoost) {
        color[0] = color[4] = color[8] = color[12] = 0.5;
        color[1] = color[5] = color[9] = color[13] = 1;
        color[2] = color[6] = color[10] = color[14] = 0.5;
    }
    drawTextureRect(texGBoost, mRatio10 - 1, 10, false, 1, 1, color);
    drawTextureRect(texPause, mRatio10 - 1, 1, false, 1, 1);
    drawTextureRect(texTime, 0.2f, 9.8f, false, ((float) texTime.width / texTime.height)*0.6f, 0.6f);

}

void Painter::processQueuedTextLabels() {
    for (list<TextLabelData*>::iterator it = mTextLabelQueue.begin(); it != mTextLabelQueue.end(); ++it) {
        TextLabelData *tld = *it;
        tld->target->unload();
        //tld->font->createTextLabel(*tld->target, tld->text.c_str(), tld->color, tld->multi, tld->multiW, tld->justify);
        if (tld->ptrW != nullptr && tld->ptrH != nullptr) {
            pixelToReal(tld->target->width, tld->target->height, *tld->ptrW, *tld->ptrH, false);
        }

        if (tld->lbl) {
            float tmpW, tmpH;
            pixelToReal(tld->target->width, tld->target->height, tmpW, tmpH, false);
//            tld->lbl->autoResizeWidget(tmpW, tmpH);
        }
        delete tld;
    }
    mTextLabelQueue.clear();
}

void Painter::drawFrame(GameScene *g) {
    if (quitting) {
        return;
    }
    //drawDebugStuff();
    //return;
    processQueuedTextLabels();
    for (list<WidgetData*>::iterator it = mWidgetQueue.begin(); it != mWidgetQueue.end(); ++it) {
//        (*it)->w->runInGLThread((*it)->data);
        delete *it;
    }
    mWidgetQueue.clear();

    //quick 'n' dirty as fuck
    //handle all the fade-out-and-then-in-to-text-then-wait-and-then-fade-back-out-again stuff
    if (mFading && mFadeIsText && (mFadeTextPhase >= 1)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.59f, 0.72f, 0.9f, 1.0f);
        drawTextureRect(mFadeTextTexture, mRatio10 / 2 - mFadeTextWidth / 2, 5.0f + mFadeTextHeight / 2, false, mFadeTextWidth, 0, nullptr);
        if (mFadeTextPhase != 2) {
            drawFadeout();
        }
        return;
    }

    //here starts the semi-normal part (I swear I will refactor this code some day)
    if (!g->inMenu) {
        if (mRequestedSnapshot) {
            glBindFramebuffer(GL_FRAMEBUFFER, mSnapshotFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texGameSnapshot.id, 0);
        }

        if (!mGameSnapshot || mRequestedSnapshot) {
            drawInterface(g);
        } else {
            glDisable(GL_BLEND);
            drawTextureRect(texGameSnapshot, 0, 0, false, mRatio10, -10);
            glEnable(GL_BLEND);
        }

        if (mRequestedSnapshot) {
            glBindFramebuffer(GL_FRAMEBUFFER, mDefaultFBO);
            mRequestedSnapshot = false;
        }

        if (g->overlayMenu) {
//            drawMenu(g->getMenu());
        }
    }

    if (g->showFPS) {
        drawTextureRect(texFPS, mRatio10 - 2, 9.6f, false, 1.7f, 1.7f * (texFPS.height / (float) texFPS.width));
    }

    if (mFading) {
        drawFadeout();
    }
}

void Painter::requestGameSnapshot() {
    mRequestedSnapshot = true;
}

void Painter::displayGameSnapshot(bool show) {
    mGameSnapshot = show;
}

void Painter::updateTimeTextures(float fps, int timesteps, bool showFPS) {
    char fpsstr[16];
    if (showFPS) {
        snprintf(fpsstr, 16, "FPS: %.1f", fps);
        texFPS.unload();
///        mainFont.createTextLabel(texFPS, fpsstr);
    }

    int seconds = timesteps / 1000;
    int minutes = seconds / 60;
    seconds -= minutes * 60;
    snprintf(fpsstr, 15, "%d:%.2d", minutes, seconds);
    texTime.unload();
    int timeColor = activeBkg ? 0xFFFFFFFF : 0;
//    mainFont.createTextLabel(texTime, fpsstr, timeColor);
}

void Painter::pixelToReal(int px, int py, float &rx, float &ry, bool invertY) {
    rx = px * mRatio * 10.0f / mScreenW;
    if (invertY) {
        ry = 10.0f - (py * 10.0f) / mScreenH;
    }else{
        ry = (py * 10.0f) / mScreenH;
    }
}

void Painter::realToPixel(float rx, float ry, int &px, int &py, bool invertY) {
    px = rx / (mRatio * 10.0f / mScreenW);
    if (invertY) {
        py = (10.0f - ry) * mScreenH / 10.0f;
    }else{
        py = (ry * mScreenH) / 10.0f;
    }
}

bool Painter::ReportFixture(b2Fixture *fixture) {
    World::MyUserData *data = (World::MyUserData*)fixture->GetUserData();
    if (!data) {
        return true;
    }

    if (data->type == World::MyUserData::EnemyType) {
        mDrawnAABB.insert(data->enemy);
    } else if (data->type == World::MyUserData::RailType) {
        mDrawnAABB.insert(data->rail);
    } else if (data->type == World::MyUserData::StaticType) {
        mDrawnAABB.insert(data->staticObject);
    }
    return true;
}

void Painter::drawDebugStuff() {

}

GLfloat *Painter::toGlArrayXYZ(const vector<Vec2>& points, GLfloat z) {
    GLfloat *arr = new GLfloat[points.size() * 3];
    int i = 0;
    for (vector<Vec2>::const_iterator it = points.begin(); it != points.end(); ++it) {
        arr[i++] = it->x();
        arr[i++] = it->y();
        arr[i++] = z;
    }
    return arr;
}

void Painter::dumpVectors(const vector<Vec2>& points, const string& prefix) {
    cout << prefix << endl;
    for (vector<Vec2>::const_iterator it = points.begin(); it != points.end(); ++it) {
        const Vec2 &vec = *it;
        cout << "[" << vec.x() << ", " << vec.y() << "]" << endl;
    }
    cout << "---" << endl;
}
