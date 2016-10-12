#include "Settings.h"
#include "Logging.h"
#include <expat.h>
#include <cstdlib>
#include <cstdio>
#include "platform/core.h"

using namespace std;
using namespace RailClimberPlatform;

std::map<std::string, std::string> Settings::mValues;
int Settings::mLevel;
std::string Settings::mFirstLevel, Settings::mSecondLevel;
RCLSettingsType Settings::mSaved;

void Settings::startXmlElement(void *userData, const char *name, const char **atts) {
    mLevel++;
    if (mLevel == 2) {
        mFirstLevel = name;
    } else if (mLevel == 3) {
        mSecondLevel = name;
    }
}

void Settings::endXmlElement(void *userData, const char *name) {
    mLevel--;
}

void Settings::charDataHandler(void *userData, const XML_Char *s, int len) {
    if (mLevel == 3) {
        mValues[mFirstLevel + "/" + mSecondLevel] = string(s, len);
    }
}

bool Settings::load(const string &filename) {
    int len;
    char *buf = loadFile(filename.c_str(), &len);
    if (!buf) {
        Log::error("Couldn't load settings file from %s", filename.c_str());
        return false;
    }

    XML_Parser parser = XML_ParserCreate(nullptr);
    XML_SetElementHandler(parser, startXmlElement, endXmlElement);
    XML_SetCharacterDataHandler(parser, charDataHandler);

    mLevel = 0;
    int done = 0;
    do {
        done = true;
        if (!XML_Parse(parser, buf, len, done)) {
            Log::error("XML parse error");
            delete[] buf;
            return false;
        }
    } while (!done);
    delete[] buf;
    XML_ParserFree(parser);
    return true;
}

int Settings::getInt(const string &key, int def) {
    if (mValues.find(key) == mValues.end()) {
        Log::warn("No such key '%s', returning %d", key.c_str(), def);
        return def;
    }
    return strtol(mValues[key].c_str(), nullptr, 0);
}

float Settings::getFloat(const string &key, float def) {
    if (mValues.find(key) == mValues.end()) {
        Log::warn("No such key '%s', returning %f", key.c_str(), def);
        return def;
    }
    return strtod(mValues[key].c_str(), nullptr);
}

string Settings::getString(const string &key, const string &def) {
    if (mValues.find(key) == mValues.end()) {
        Log::warn("No such key '%s', returning '%s'", key.c_str(), def.c_str());
        return def;
    }
    return mValues[key];
}

bool Settings::getBool(const string &key, bool def) {
    if (mValues.find(key) == mValues.end()) {
        Log::warn("No such key '%s', returning %d", key.c_str(), int(def));
        return def;
    }

    const string &v = mValues[key];
    if (v[0] == 't' || v[0] == 'T' || v[0] == '1' || (v[0] == 'o' && v[1] == 'n')) {
        return true;
    }
    return false;
}

void Settings::loadSaved(const string &filename) {
    mSaved = loadSettings(filename.c_str());
}

int Settings::getSavedInt(const string &section, const string &key, int def) {
    string c = getSetting(mSaved, section.c_str(), key.c_str());
    if (c.empty()) {
        return def;
    }
    return strtol(c.c_str(), nullptr, 0);
}

float Settings::getSavedFloat(const string &section, const string &key, float def) {
    string c = getSetting(mSaved, section.c_str(), key.c_str());
    if (c.empty()) {
        return def;
    }
    return strtod(c.c_str(), nullptr);
}

//TODO: toto neumoznuje ulozit empty string ak je iny default

string Settings::getSavedString(const string &section, const string &key, const string &def) {
    string c = getSetting(mSaved, section.c_str(), key.c_str());
    if (c.empty()) {
        return def;
    }
    return c;
}

bool Settings::getSavedBool(const string &section, const string &key, bool def) {
    string v = getSetting(mSaved, section.c_str(), key.c_str());
    if (v.empty()) {
        return def;
    }
    if (v[0] == 't' || v[0] == 'T' || v[0] == '1' || (v[0] == 'o' && v[1] == 'n')) {
        return true;
    }
    return false;
}

void Settings::setSavedInt(const string &section, const string &key, int val) {
    char c[64];
    snprintf(c, 63, "%d", val);
    setSetting(mSaved, section.c_str(), key.c_str(), c);
}

void Settings::setSavedFloat(const string &section, const string &key, float val) {
    char c[512];
    snprintf(c, 511, "%f", val);
    setSetting(mSaved, section.c_str(), key.c_str(), c);
}

void Settings::setSavedString(const string &section, const string &key, const string &val) {
    setSetting(mSaved, section.c_str(), key.c_str(), val.c_str());
}

void Settings::setSavedBool(const string &section, const string &key, bool val) {
    setSetting(mSaved, section.c_str(), key.c_str(), (val ? "1" : "0"));
}

void Settings::unloadSaved() {
    unloadSettings(mSaved);
}
