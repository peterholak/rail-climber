#pragma once

#include <stdarg.h>
#include "platform/core.h"

class Log {
public:

    enum LogLevel {
        Debug = 0, Info = 1, Notice = 2, Warning = 3, Error = 4
    };

    static void info(const char *fmt, ...) {
        va_list argList;
        va_start(argList, fmt);
        log(Info, fmt, argList);
        va_end(argList);
    }

    static void notice(const char *fmt, ...) {
        va_list argList;
        va_start(argList, fmt);
        log(Notice, fmt, argList);
        va_end(argList);
    }

    static void debug(const char *fmt, ...) {
        va_list argList;
        va_start(argList, fmt);
        log(Debug, fmt, argList);
        va_end(argList);
    }

    static void warn(const char *fmt, ...) {
        va_list argList;
        va_start(argList, fmt);
        log(Warning, fmt, argList);
        va_end(argList);
    }

    static void error(const char *fmt, ...) {
        va_list argList;
        va_start(argList, fmt);
        log(Error, fmt, argList);
        va_end(argList);
    }

    static int logLevel;

private:

    static void log(int level, const char *fmt, va_list argList) {
        if (level >= logLevel) {
            RailClimberPlatform::log(level, fmt, argList);
        }
    }
};
