#pragma once

#include "SandboxCase.h"
class Label;
class StaticPicture;
class Button;

class GuiCase : public SandboxCase {
    using SandboxCase::SandboxCase;

public:
    virtual void initializeCase();
    virtual void drawCase(const BitmapRenderer &bitmapRenderer);
    virtual void handleCaseEvent(const SDL_Event &event);

private:
    Label *mLabel;
    StaticPicture *mStaticPicture;
    Button *mButton;

    void onButtonClicked(Button *sender);
};
