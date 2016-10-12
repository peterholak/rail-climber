#include "ExpatParser.h"

ExpatParser::ExpatParser() {
}

ExpatParser::~ExpatParser() {
}

void ExpatParser::xmlParse(const char *buf, size_t len) {
    mParser = XML_ParserCreate(nullptr);

    mElementStack.clear();

    XML_SetUserData(mParser, this);
    XML_SetElementHandler(mParser, ExpatParser::helperXmlElementStart, ExpatParser::helperXmlElementEnd);
    XML_SetCharacterDataHandler(mParser, ExpatParser::helperCharDataHandler);

    if (!XML_Parse(mParser, buf, len, true)) {
        XML_Error errorCode = XML_GetErrorCode(mParser);
        throw XMLException(errorCode, XML_ErrorString(errorCode));
    }

    XML_ParserFree(mParser);
}

void ExpatParser::helperXmlElementStart(void* userData, const char* name, const char** atts) {
    ExpatParser *parser = static_cast<ExpatParser*> (userData);
    std::map<std::string, std::string> attMap;

    int i = 0;
    while (atts[i] != nullptr) {
        attMap[atts[i]] = atts[i + 1];
        i += 2;
    }

    parser->mElementStack.push_back(name);
    parser->onXmlElementStart(name, attMap);
}

void ExpatParser::helperXmlElementEnd(void* userData, const char* name) {
    ExpatParser *parser = static_cast<ExpatParser*> (userData);
    //let's just assume we have a valid xml and not worry about this shit for now
    parser->mElementStack.pop_back();
    parser->onXmlElementEnd(name);
}

void ExpatParser::helperCharDataHandler(void* userData, const XML_Char* s, int len) {
    ExpatParser *parser = static_cast<ExpatParser*> (userData);
    parser->onXmlCharacterData(std::string(s, len));
}

const std::string& ExpatParser::lastElement() {
    return mElementStack.back();
}

const std::vector<std::string>& ExpatParser::elementStack() {
    return mElementStack;
}
