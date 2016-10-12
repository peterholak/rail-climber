#pragma once

#include <map>
#include <string>

class IFadeCallback {
public:
    virtual void fadeCallback(const std::map<std::string, std::string> &callbackMap, bool cancelled) = 0;

    virtual ~IFadeCallback() { }
};
