#pragma once

#include "ui/AnimationWidget.h"

class AnimationLevel7 : public AnimationWidget {
public:
    AnimationLevel7(Painter *painter, MenuScreen *menu, const std::string &_name, float _x, float _y, float _w, float _h);
    virtual ~AnimationLevel7();

    void draw() { }
    void reload() { }
    void unload() { }
    void tick(bool debugManual = false) { }
    void screenInit() { }
    void runInGLThread(void *data = NULL) { }

private:
    Vec2 mFingerPos;
};
