#pragma once

#include <map>
#include <string>
#include "platform/core.h"

class Settings {
public:
    static bool load(const std::string &filename);
    static int getInt(const std::string &key, int def = 0);
    static float getFloat(const std::string &key, float def = 0);
    static std::string getString(const std::string &key, const std::string &def = "");
    static bool getBool(const std::string &key, bool def = false);

    static void loadSaved(const std::string &filename);
    static int getSavedInt(const std::string &section, const std::string &key, int def = 0);
    static float getSavedFloat(const std::string &section, const std::string &key, float def = 0);
    static std::string getSavedString(const std::string &section, const std::string &key, const std::string &def = "");
    static bool getSavedBool(const std::string &section, const std::string &key, bool def = false);
    static void setSavedInt(const std::string &section, const std::string &key, int val);
    static void setSavedFloat(const std::string &section, const std::string &key, float val);
    static void setSavedString(const std::string &section, const std::string &key, const std::string &val);
    static void setSavedBool(const std::string &section, const std::string &key, bool val);
    static void unloadSaved();

private:
    static std::map<std::string, std::string> mValues;
    static int mLevel;
    static std::string mFirstLevel, mSecondLevel;
    static RailClimberPlatform::RCLSettingsType mSaved;
    static void startXmlElement(void *userData, const char *name, const char **atts);
    static void endXmlElement(void *userData, const char *name);
    static void charDataHandler(void *userData, const char *s, int len);
};
