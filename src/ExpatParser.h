#pragma once

// to stop NetBeans's code assistance from complaining
// about some functions being undefined
#ifdef __MINGW32__
#define XMLCALL
#endif

#include <expat.h>
#include <string>
#include <map>
#include <vector>

class XMLException : public std::exception {
public:

    XMLException(XML_Error errorCode, const std::string& errorString) : mErrorCode(errorCode), mErrorString(errorString) {
    }

    virtual const char* what() const noexcept {
        return mErrorString.c_str();
    }

    std::string errorString() {
        return mErrorString;
    }

    XML_Error errorCode() {
        return mErrorCode;
    }

private:
    XML_Error mErrorCode;
    std::string mErrorString;
};

class ExpatParser {
public:
    ExpatParser();
    virtual ~ExpatParser();

    void xmlParse(const char *buf, size_t len);

    virtual void onXmlElementStart(const std::string &name, const std::map<std::string, std::string> &atts) = 0;
    virtual void onXmlElementEnd(const std::string &name) = 0;
    virtual void onXmlCharacterData(const std::string &data) = 0;

protected:
    const std::string &lastElement();
    const std::vector<std::string> &elementStack();

    XML_Parser mParser;
    std::vector<std::string> mElementStack;

private:
    static void helperXmlElementStart(void *userData, const char *name, const char **atts);
    static void helperXmlElementEnd(void *userData, const char *name);
    static void helperCharDataHandler(void *userData, const XML_Char *s, int len);
};
