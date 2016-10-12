#include "platform/core.h"
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <SDL_image.h>
#include <sys/stat.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <sys/time.h>
#include "game_logic/GameScene.h"
#include "Logging.h"

using namespace std;

namespace RailClimberPlatform {

    string lastLogMessage;
    int logMessageRepeatCount = 0;
    string dataDir = "./";
    SDL_Window *g_sdlWindow = nullptr;

    void initialize() {
        struct stat st;

        string searchDirs[] = {
            "./",
            "../data/",
            "../../data/",
            "data/"
        };

        for (const string &s : searchDirs) {
            if (stat((s + "railclimber.xml").c_str(), &st) == 0) {
                dataDir = s;
                break;
            }
        }
    }

    /**
     * Logs a message in printf-like format
     */
    void log(int level, const char *fmt, va_list argList) {
        time_t t = time(nullptr);
        tm *lt = localtime(&t);
        timeval tp;
        gettimeofday(&tp, nullptr);

        char timestr[16];
        snprintf(timestr, 14, "%02d:%02d:%02d.%03d ", lt->tm_hour, lt->tm_min, lt->tm_sec, int(tp.tv_usec / 1000));

        char errorStr[16];
        switch (level) {
            case 0: strncpy(errorStr, "DEBUG:   ", 16);
                break;
            case 1: strncpy(errorStr, "INFO:    ", 16);
                break;
            case 2: strncpy(errorStr, "NOTICE:  ", 16);
                break;
            case 3: strncpy(errorStr, "WARNING: ", 16);
                break;
            case 4: strncpy(errorStr, "ERROR:   ", 16);
                break;
        }

        constexpr size_t bufferSize = 1024;
        char buffer[bufferSize];
        vsnprintf(buffer, bufferSize, fmt, argList);
        buffer[bufferSize - 1] = 0;

        if (lastLogMessage == buffer) {
            logMessageRepeatCount++;
            if (logMessageRepeatCount == 1000) {
                cout << "Last log message repeated 1000 times" << endl;
                logMessageRepeatCount = 0;
            }
        }else {
            string message = string(timestr) + errorStr + buffer + "\n";
            lastLogMessage = buffer;
            cout << message;
            fflush(stdout);
        }
    }

    bool g_quit = false;
    SDL_TimerID vibTimer;
    int vibCounter = 0;

    Uint32 VibTimerProc(Uint32 interval, void *data) {
        vibCounter++;
        if (vibCounter == 10) {
            vibCounter = 0;
            SDL_RemoveTimer(vibTimer);
            SDL_SetWindowTitle(g_sdlWindow, "RailClimber");
        }
        return interval;
    }

    char *loadFile(const char *filename, int *lenret) {
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

    void quit() {
        g_quit = true;
    }

    RCLSettingsType loadSettings(const char *filename) {
        RCLSDLSettings *settings = new RCLSDLSettings();

        settings->filename = dataDir + filename;
        settings->filename += ".save";

        ifstream f(settings->filename.c_str());
        if (!f.is_open())
            return settings;

        char line[1027];
        while (!f.eof()) {
            f.getline(line, 1024);
            line[f.gcount() - 1] = 0;
            string s = line;

            int tabpos = s.find('\t');
            string ks = s.substr(0, tabpos);
            string val = s.substr(tabpos + 1);
            settings->settings[ks] = val;
        }

        f.close();
        return settings;
    }

    void _commitSettings(RCLSettingsType settings) {
        ofstream f(settings->filename.c_str());
        for (map<string, string>::iterator it = settings->settings.begin(); it != settings->settings.end(); ++it) {
            f << it->first << '\t' << it->second << '\n';
        }
        f.close();
    }

    void setSetting(RCLSettingsType settings, const char *section, const char *key, const char *value) {
        string s = string(section) + "/" + key;
        settings->settings[s] = value;
        _commitSettings(settings);
    }

    string getSetting(RCLSettingsType settings, const char *section, const char *key) {
        string s = string(section) + "/" + key;
        if (settings->settings.find(s) != settings->settings.end()) {
            return settings->settings[s];
        }
        return string();
    }

    void unloadSettings(RCLSettingsType settings) {
        _commitSettings(settings);
        delete settings;
    }

    void vibrate(int duration) {
        if (vibCounter == 0) {
            vibTimer = SDL_AddTimer(duration / 10, VibTimerProc, nullptr);
            SDL_SetWindowTitle(g_sdlWindow, "~~~~~~~~ RailClimber ~~~~~~~~");
        }
    }

    string getLanguageCode() {
        ifstream f((dataDir + "railclimber.lang").c_str());
        if (!f.is_open())
            return "en";
        string s;
        f >> s;
        f.close();
        return s;
    }

    long long getTicks() {
        timeval tp;
        gettimeofday(&tp, nullptr);
        return tp.tv_sec * 1000 + tp.tv_usec / 1000;
    }

}
