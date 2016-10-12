#include "AnimationLevel7.h"
#include "ui/MenuScreen.h"

AnimationLevel7::AnimationLevel7(Painter *painter, MenuScreen *menu, const std::string &_name, float _x, float _y, float _w, float _h)
: AnimationWidget(painter, _name, _x, _y, _w, _h) {
    mMenu = menu;

}

AnimationLevel7::~AnimationLevel7() {

}
