#include "Translation.h"
#include "Logging.h"
#include "platform/core.h"
#include <expat.h>

using namespace std;

map<string, string> Tr::table;
string Tr::mLastOrig;
char Tr::mLastTag;
bool Tr::mEnglish;

Tr::Tr() {

}

Tr::~Tr() {

}

void Tr::startXmlElement(void *userData, const char *name, const char **atts) {
    mLastTag = name[0];
}

void Tr::endXmlElement(void *userData, const char *name) {
    mLastTag = 0;
}

void Tr::charDataHandler(void *userData, const char *s, int len) {
    if (mLastTag == 'O') {
        mLastOrig = string(s, 0, len);
    }else if (mLastTag == 'T'){
        table[mLastOrig] = string(s, 0, len);
    }
}

void Tr::initialize() {
    Log::info("Loading file for language '%s'", RailClimberPlatform::getLanguageCode().c_str());
    mEnglish = true;
    //mEnglish = (RailClimberPlatform::getLanguageCode() == "en");
    if (mEnglish) {
        return;
    }

    int len;
    char *buf = RailClimberPlatform::loadFile((string("languages/") + RailClimberPlatform::getLanguageCode() + ".xml").c_str(), &len);
    if (!buf) {
        Log::error("Tr: Couldn't load language file from languages/%s.xml", RailClimberPlatform::getLanguageCode().c_str());
        mEnglish = true;
        return;
    }

    XML_Parser parser = XML_ParserCreate(nullptr);
    XML_SetElementHandler(parser, startXmlElement, endXmlElement);
    XML_SetCharacterDataHandler(parser, charDataHandler);

    int done = 0;
    do {
        done = true;
        if (!XML_Parse(parser, buf, len, done)) {
            Log::error("Tr: XML parse error");
            delete[] buf;
            mEnglish = true;
            return;
        }
    } while (!done);
    delete[] buf;
    XML_ParserFree(parser);
}

const string &Tr::tr(const string &english) {
    if (mEnglish) {
        return english;
    }
    map<string, string>::iterator it = table.find(english);
    if (it == table.end()) {
        return english;
    }
    return it->second;
}
