#include "RailClimberApp.h"
#include <cstring>
#include "dev_sandbox/RailClimberSandbox.h"
#include "platform_specific/sdl/dev_sandbox/GuiCase.h"
#include "platform_specific/sdl/dev_sandbox/YamlLevelCase.h"

int main(int argc, char *argv[]) {
    if (argc > 1 && !std::strcmp(argv[1], "test")) {
        RailClimberSandbox testApp;
        testApp.setSandboxCase<GuiCase>();
        // testApp.setSandboxCase<YamlLevelCase>();
        return testApp.run();
    }

    RailClimberApp app("railclimber.xml");
    return app.run();
}
