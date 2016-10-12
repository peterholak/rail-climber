#include "../../platform/core.h"
#include <FMedia.h>
#include <string>
#include <list>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <FBase.h>
#include <FSystem.h>
#include <FIo.h>
#include <FApp.h>
#include <FLocales.h>
#include "RailClimber.h"
#include "../../Logging.h"

using namespace std;
using namespace Tizen::Base;
using namespace Tizen::Io;
using namespace Tizen::System;

namespace RailClimberPlatform {

RailClimber *g_app;
//Tizen::System::Vibrator *g_vibrator;
//std::string g_locale;

char *loadFile(const char *filename, int *lenret) {
	char dataPath[1024];
	const wchar_t *wData = g_app->GetAppDataPath().GetPointer();
	wcstombs(dataPath, wData, 1024);
	string s = string(dataPath) + "/";

	s += filename;
    struct stat st;
    int stres = stat(s.c_str(), &st);
    if (stres < 0)
    	return NULL;
    size_t len = st.st_size;
    char *buf = new char[len + 1];
    buf[len] = 0;
    FILE *f = fopen(s.c_str(), "rb");
    if (!f) {
    	AppLog("Can't open file %s", filename);
    	return NULL;
    }
    fread(buf, 1, len, f);
    fclose(f);
    if (lenret)
        *lenret = len;
    return buf;
}

std::string getLanguageCode() {
	/*if (g_locale.empty()) {
		Tizen::Locales::LocaleManager lm;
		lm.Construct();
		Tizen::Base::String s = lm.GetSystemLocale().GetLanguageCodeString();
		if (s.Equals("slk", false) || s.Equals("slo", false))
			g_locale = "sk";
		else g_locale = "en";
	}
	return g_locale;*/
	return "en";
}

void quit() {
	g_app->Terminate();
}

RCLSettingsType loadSettings(const char *filename) {
	Tizen::Io::Registry *settings = new Tizen::Io::Registry;
	char resFn[1024];
	AppLog("Loading settings...");

	char dataPath[1024];
	const wchar_t *wData = g_app->GetAppDataPath().GetPointer();
	wcstombs(dataPath, wData, 1024);
	string s = string(dataPath);

	snprintf(resFn, 1023, "%s/%s.save", s.c_str(), filename);
	AppLog("Loading saved settings from %s", resFn);
	result r = settings->Construct(resFn, true);
	if (IsFailed(r)) {
		AppLog("Settings construct failed: %s", GetErrorMessage(r));
	}
	return settings;
}

void setSetting(RCLSettingsType settings, const char *section, const char *key, const char *value) {
	settings->AddSection(section);
	Tizen::Base::ByteBuffer bb;
	bb.Construct(1024);
	bb.SetArray((const byte*)value, 0, strlen(value));
	result r = settings->AddValue(section, key, bb);
	if (r == E_KEY_ALREADY_EXIST) {
		settings->SetValue(section, key, bb);
	}
	settings->Flush();
}

std::string getSetting(RCLSettingsType settings, const char *section, const char *key) {
	Tizen::Base::ByteBuffer bb;
	bb.Construct(1024);
	byte ar[1024];
	settings->GetValue(section, key, bb);
	bb.GetArray(ar, 0, bb.GetRemaining());
	return (const char*)ar;
}

void unloadSettings(RCLSettingsType settings) {
	delete settings;
}

void vibrate(int duration) {
	/*IntensityDurationVibrationPattern pattern;
	pattern.duration = duration;
	pattern.intensity = -1;
	g_vibrator->Start(&pattern, 1);*/
	//g_app->mVibrateWorkaroundTimer->Start(duration);
}

long long getTicks() {
	long long ticks;
	Tizen::System::SystemTime::GetTicks(ticks);
	return ticks;
}

void log(int level, const char *fmt, va_list argList) {
	char out[1025];
	vsnprintf(out, 1024, fmt, argList);
	if (level == 0)
		AppLogDebugTag("RailClimber", "%s", out);
	else if (level == 3 || level == 4)
		AppLogExceptionTag("RailClimber", "%s", out);
	else
		AppLogTag("RailClimber", "%s", out);
}

}
