#pragma once

class IAnimationCallback {
public:
    //TODO: some actual data structure
    virtual void animationCallback(int block) = 0;
    virtual ~IAnimationCallback() { }
};
