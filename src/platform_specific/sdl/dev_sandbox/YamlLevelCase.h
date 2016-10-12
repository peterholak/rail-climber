#pragma once

#include "SandboxCase.h"

class BitmapRenderer;

class YamlLevelCase : public SandboxCase {
    using SandboxCase::SandboxCase;

    virtual void initializeCase();
    virtual void drawCase(const BitmapRenderer &bitmapRenderer) { }
};


