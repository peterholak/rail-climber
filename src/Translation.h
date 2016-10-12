#pragma once

#include <string>
#include <map>
#define _(x) Tr::tr((x))

class Tr {
public:
    Tr();
    virtual ~Tr();

    static void initialize();
    static const std::string &tr(const std::string &english);

private:
    static std::map<std::string, std::string> table;
    static void startXmlElement(void *userData, const char *name, const char **atts);
    static void endXmlElement(void *userData, const char *name);
    static void charDataHandler(void *userData, const char *s, int len);
    static std::string mLastOrig;
    static char mLastTag;
    static bool mEnglish;
};
