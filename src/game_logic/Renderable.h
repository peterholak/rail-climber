#pragma once

class RenderDriverInterface;

class Renderable {
public:
    virtual void render(RenderDriverInterface *renderDriver) = 0;
    virtual ~Renderable() { }

protected:
    int mZIndex;
};
