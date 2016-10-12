#pragma once

#include <FApp.h>
#include <FIo.h>
#include <FSystem.h>

class RailClimber;

namespace RailClimberPlatform {
    extern RailClimber *g_app;
    //extern Tizen::System::Vibrator *g_vibrator;
    typedef Tizen::Io::Registry* RCLSettingsType;
}
