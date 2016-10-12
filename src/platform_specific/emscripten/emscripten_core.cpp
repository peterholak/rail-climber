#include "emscripten_core.h"
#include <string>
#include <cstdio>
#include <SDL_video.h>
#include <sys/stat.h>
#include "Logging.h"

using namespace std;

namespace RailClimberPlatform {

    SDL_Window *g_sdlWindow = nullptr;
    bool g_quit = false;

    void initialize() { }

    void log(int level, const char *fmt, va_list argList) {
        vprintf(fmt, argList);
        printf("\n");
        fflush(stdout);
    }

    /**
     * Load a file. The returned buffer has to be manually freed using delete[]
     */
    char *loadFile(const char *filename, int *lenret) {
        string dataDir = "../../data/";
        string fn = dataDir + filename;
        struct stat st;

        int statResult = stat(fn.c_str(), &st);
        if (statResult < 0) {
            Log::error("Can't load file from '%s'", fn.c_str());
            return nullptr;
        }

        size_t len = st.st_size;
        char *buf = new char[len + 1];
        buf[len] = 0;
        FILE *f = fopen(fn.c_str(), "rb");
        fread(buf, 1, len, f);
        fclose(f);
        if (lenret)
            *lenret = len;
        return buf;
    }

    /**
     * Immediately exit the game in a platform-specific way
     */
    void quit() { }

    RCLSettingsType loadSettings(const char *filename) { return nullptr; }
    void unloadSettings(RCLSettingsType settings) { }
    void setSetting(RCLSettingsType settings, const char *section, const char *key, const char *value) { }
    std::string getSetting(RCLSettingsType settings, const char *section, const char *key) { return ""; }

    std::string getLanguageCode() { return "en"; }
    void vibrate(int duration) { }
    long long getTicks() { return 0; }

}
