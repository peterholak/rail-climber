#include "SceneManager.h"

using namespace std;

void SceneManager::pushScene(SceneInterface *scene) {
    // TODO: is this wise? check whether the scene that called this is actually the top scene? or just assert it?
    scene->finished.connect<SceneManager, &SceneManager::popScene>(this);
    scene->sceneChangeRequested.connect<SceneManager, &SceneManager::sceneChangeRequested>(this);
    mSceneStack.push_back(scene);
}

void SceneManager::popScene() {
    // TODO: this is not good at all, for performance reasons if nothing else
    SceneInterface *top = topScene();
    top->finished.disconnect<SceneManager, &SceneManager::popScene>(this);
    mSceneStack.pop_back();
}

SceneInterface* SceneManager::topScene() {
    return mSceneStack.back();
}

SceneManager::SceneManager(const UnitConverter &unitConverter) : mUnitConverter(unitConverter) {

}

void SceneManager::handleTouchPress(int pixelX, int pixelY) {
    Vec2 inGameXY = mUnitConverter.inGameXY(Vec2G<int>(pixelX, pixelY), Vec2());
    for (auto it = mSceneStack.rbegin(); it != mSceneStack.rend(); ++it) {
        if ((*it)->handleTouchPress(inGameXY)) {
            break;
        }
    }
}

void SceneManager::handleTouchMove(int pixelX, int pixelY) {
    Vec2 inGameXY = mUnitConverter.inGameXY(Vec2G<int>(pixelX, pixelY), Vec2());
    for (auto it = mSceneStack.rbegin(); it != mSceneStack.rend(); ++it) {
        if ((*it)->handleTouchMove(inGameXY)) {
            break;
        }
    }
}

void SceneManager::handleTouchRelease(int pixelX, int pixelY) {
    Vec2 inGameXY = mUnitConverter.inGameXY(Vec2G<int>(pixelX, pixelY), Vec2());
    for (auto it = mSceneStack.rbegin(); it != mSceneStack.rend(); ++it) {
        if ((*it)->handleTouchRelease(inGameXY)) {
            break;
        }
    }
}

void SceneManager::handleBackAction() {
    for (auto it = mSceneStack.rbegin(); it != mSceneStack.rend(); ++it) {
        if ((*it)->handleBackAction()) {
            break;
        }
    }
}

void SceneManager::replaceStack(SceneInterface *scene) {
    // TODO: lol performance
    while (!mSceneStack.empty()) {
        mSceneStack.pop_back();
    }
    mSceneStack.push_back(scene);
}

void SceneManager::renderScene() {
    if (mSceneStack.empty()) {
        return;
    }

    int topOpaqueScenePosition = 0;

    for (int i = int(mSceneStack.size()) - 1; i >= 0; i--) {
        if (!mSceneStack[i]->isTransparent()) {
            topOpaqueScenePosition = i;
            break;
        }
    }

    for (unsigned int i = topOpaqueScenePosition; i < mSceneStack.size(); i++) {
        mSceneStack[i]->render();
    }
}

bool SceneManager::sceneChangeRequested(const char *sceneId, SceneInterface::SceneChangeType changeType) {
    assert(mRegisteredScenes.find(sceneId) != mRegisteredScenes.end());
    SceneInterface *scene = mRegisteredScenes[sceneId];
    switch (changeType) {
        case SceneInterface::Push:
            pushScene(scene);
            break;

        case SceneInterface::Replacement:
            replaceStack(scene);
            break;
    }
    return true;
}

void SceneManager::registerScene(SceneInterface *scene) {
    mRegisteredScenes[scene->getSceneId()] = scene;
}
