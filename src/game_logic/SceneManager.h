#pragma once

#include <vector>
#include <map>
#include <string>
#include "SceneInterface.h"
#include "UnitConverter.h"

class SceneManager {
public:
    SceneManager(const UnitConverter &unitConverter);

    void pushScene(SceneInterface *scene);
    void registerScene(SceneInterface *scene);
    void popScene();
    void replaceStack(SceneInterface *scene);
    SceneInterface* topScene();

    void renderScene();

    void handleTouchPress(int pixelX, int pixelY);
    void handleTouchMove(int pixelX, int pixelY);
    void handleTouchRelease(int pixelX, int pixelY);
    void handleBackAction();

private:
    // TODO: should scenes be shared pointers? sure would make cleanup a lot easier...
    std::vector<SceneInterface*> mSceneStack;
    std::map<std::string, SceneInterface*> mRegisteredScenes;
    const UnitConverter &mUnitConverter;

    bool sceneChangeRequested(const char *sceneId, SceneInterface::SceneChangeType changeType);
};
