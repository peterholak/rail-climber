#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>

class Font;
class Theme;
class Texture;
class ShaderProgram;
class UnitConverter;

class SharedAssetRegistry {
public:
    SharedAssetRegistry(const UnitConverter &unitConverter);

    void loadPrograms();
    void loadUiTextures();

    const ShaderProgram &getProgram(int programId) const { return *mPrograms[programId]; }
    const ShaderProgram &defaultProgram() const { return *mPrograms[defaultProgramId]; }
    const Font &defaultFont() const { return *mFonts[defaultFontId]; }

    const Texture& getUiTexture(const std::string &textureName) const;

private:
    std::map<std::string, Texture*> mUiTextures;
    std::vector<ShaderProgram*> mPrograms;
    std::vector<Font*> mFonts;
    Theme *mCurrentTheme;
    const UnitConverter &mUnitConverter;

    void loadUpDownTextures(const std::string &name, bool includeDisabled = false);
    void loadUiTexture(const std::string &name);

public:
    static int defaultProgramId;
    static int defaultFontId;

    void loadFonts();
};
