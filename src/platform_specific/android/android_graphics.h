#ifndef ANDROID_GRAPHICS_H_
#define ANDROID_GRAPHICS_H_

#include <GLES2/gl2.h>
#include <string>

namespace RailClimberPlatform {

    struct FontStruct {
        std::string name;
        int ptsize;
    };

    typedef FontStruct* RCLFontType;

}


#endif
