#include "SharedAssetRegistry.h"
#include "graphics/Texture.h"
#include "graphics/ShaderProgram.h"
#include "graphics/Font.h"
#include "game_logic/GameScene.h"
#include "ui/screens/LevelSelectScreen.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Logging.h"

using namespace std;

int SharedAssetRegistry::defaultProgramId;
int SharedAssetRegistry::defaultFontId;

SharedAssetRegistry::SharedAssetRegistry(const UnitConverter &unitConverter) : mUnitConverter(unitConverter) {
    loadPrograms();
    loadFonts();
    loadUiTextures();
}

void SharedAssetRegistry::loadPrograms() {
    SharedAssetRegistry::defaultProgramId = mPrograms.size();
    ShaderProgram *defaultProgram = new ShaderProgram("shaders/main.vert", "shaders/main.frag");
    mPrograms.push_back(defaultProgram);

    float mScaleRatio = 1.0f; // TODO
    glm::mat4 projection = glm::ortho(0.0f, mUnitConverter.gameWidth() * mScaleRatio, 0.0f,
                                      mUnitConverter.gameHeight() * mScaleRatio, -1.0f, 1.0f);
    float *projectionValues = glm::value_ptr<float>(projection);

    defaultProgram->bind();
    defaultProgram->setUniformMatrix4("projection", projectionValues);
    defaultProgram->setUniform("u_scroll", 0.0f, 0.0f);
    defaultProgram->setUniform("noTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    defaultProgram->setUniform("texture", 0);
}

void SharedAssetRegistry::loadUiTextures() {
    loadUpDownTextures("back");
    loadUpDownTextures("on");
    loadUpDownTextures("off");
    loadUpDownTextures("level", true);
    loadUpDownTextures("level-passed");

    mUiTextures["background"] = new Texture("pictures/ui/menu-background.png");
    mUiTextures["overlay"] = new Texture("pictures/ui/menu-overlay.png");

    loadUiTexture("tiny-medal-gold");
    loadUiTexture("tiny-medal-silver");
    loadUiTexture("tiny-medal-bronze");
    mUiTextures["tiny-medal-none"] = new Texture("pictures/ui/tiny-medal-gold.png");

    loadUiTexture("medal-gold");

    loadUiTexture("gravity-boost");
    loadUiTexture("pause");
}

void SharedAssetRegistry::loadFonts() {
    SharedAssetRegistry::defaultFontId = mFonts.size();
    // TODO: does the height actually match?
    mFonts.push_back(new Font("fonts/Exo-Light.ttf", 0.6f, mUnitConverter));
}

const Texture &SharedAssetRegistry::getUiTexture(const std::string &textureName) const {
    if (mUiTextures.find(textureName) == mUiTextures.end()) {
        Log::error("No such texture: %s", textureName.c_str());
    }
    return *mUiTextures.find(textureName)->second;
}

void SharedAssetRegistry::loadUpDownTextures(const string &name, bool includeDisabled) {
    mUiTextures[name] = new Texture(string("pictures/ui/") + name + ".png");
    mUiTextures[name + "-down"] = new Texture(string("pictures/ui/") + name + "-down.png");
    if (includeDisabled) {
        mUiTextures[name + "-disabled"] = new Texture(string("pictures/ui/") + name + "-disabled.png");
    }
}

void SharedAssetRegistry::loadUiTexture(const std::string &name) {
    mUiTextures[name] = new Texture(string("pictures/ui/") + name + ".png");
}
