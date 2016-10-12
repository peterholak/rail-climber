#include "../../platform/core.h"
#include <string>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include "RailClimber.h"

using namespace std;

namespace RailClimberPlatform {

ALCcontext *context;
ALCdevice *device;
bool inited = false;

struct ALWAV {
	char *buffer;
	long bufferLen;
	ALsizei freqBuffer;
	ALenum formatBuffer;
	ALboolean loop;
	unsigned int source;
	unsigned int sampleSet;
};

int volume = 100;

void soundInit() {
	if (inited)
		return;

    device = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    alGetError();
    inited = true;
}

void soundCleanup() {
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	inited = false;
}

void *loadSound(const string &filename) {
    char dataPath[1024];
    const wchar_t *wData = g_app->GetAppDataPath().GetPointer();
    wcstombs(dataPath, wData, 1024);
    string s = string(dataPath) + "/";
    s += filename;

	ALWAV *wav = new ALWAV();
	alutLoadWAVFile(reinterpret_cast<ALbyte*>(const_cast<char*>(s.c_str())), &wav->formatBuffer, (void**)&wav->buffer, (ALsizei*)&wav->bufferLen, &wav->freqBuffer, &wav->loop);
	alGenSources(1, &wav->source);
	alGenBuffers(1, &wav->sampleSet);
	alBufferData(wav->sampleSet, wav->formatBuffer, wav->buffer, wav->bufferLen, wav->freqBuffer);
	alSourcei(wav->source, AL_BUFFER, wav->sampleSet);
	alutUnloadWAV(wav->formatBuffer, wav->buffer, wav->bufferLen, wav->freqBuffer);
	return wav;
}

void playSound(void *data) {
	ALWAV *wav = static_cast<ALWAV*>(data);
	alSourcePlay(wav->source);
}

void unloadSound(void *data) {
	ALWAV *wav = static_cast<ALWAV*>(data);
	alDeleteSources(1, &wav->source);
	alDeleteBuffers(1, &wav->sampleSet);
	delete wav;
}

void loadSilence() {

}

void stopSilence() {

}

void playSilence() {

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
