#include <SDL_mixer.h>
#include <string>
#include "sdl_core.h"

using namespace std;

namespace RailClimberPlatform {

    Mix_Music *silence;
    int volume = 50;

    void soundInit() {
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    }

    void soundCleanup() {
        Mix_CloseAudio();
    }

    void *loadSound(const string &filename) {
        string fn = filename.substr(0, filename.length() - 3);
        fn += "ogg";
        return Mix_LoadWAV((dataDir + fn).c_str());
    }

    void playSound(void *data) {
        Mix_PlayChannel(-1, (Mix_Chunk*) data, 0);
    }

    void unloadSound(void *data) {
        Mix_FreeChunk((Mix_Chunk*) data);
    }

    int getVolume() {
        return volume;
    }

    void setVolume(int vol) {
        if (vol < 0) vol = 0;
        if (vol > 100) vol = 100;
        volume = vol;
    }

}
