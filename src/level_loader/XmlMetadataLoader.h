#pragma once

#include "ExpatParser.h"
#include "LevelMetadataLoaderInterface.h"
#include <string>
#include <map>

class LevelMetadata;

class XmlMetadataLoader : public LevelMetadataLoaderInterface, public ExpatParser {
public:
    XmlMetadataLoader();
    virtual ~XmlMetadataLoader();

    virtual void onXmlElementStart(const std::string& name, const std::map<std::string, std::string>& atts);
    virtual void onXmlElementEnd(const std::string& name);
    virtual void onXmlCharacterData(const std::string& data);

    virtual LevelMetadata *loadLevelMetadata(const std::string& data);

private:
    LevelMetadata *mLevel = nullptr;
};
