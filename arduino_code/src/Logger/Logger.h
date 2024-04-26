//
// Created by thevizwiz on 3/17/2024.
//

#ifndef ARDUINO_CODE_LOGGER_H
#define ARDUINO_CODE_LOGGER_H

#include "Arduino.h"
#include "SD.h"


#define LOGGING_DIR "gamtryLogging"
#define LOGGING_BASE_NAME "gamtry_log_"
#define LOGGING_FILE_EXT ".txt"
#define LOGGING
#define LOGGING_TO_SERIAL
#define LOGGING_TIMESTAMPS
//#define LOGGING_SD_LOGS
//#define LOGGING_SD_WARNINGS
//#define LOGGING_SD_ERRORS
//#define LOGGING_SD_DEBUGS
#define LOGGING_SERIAL_LOGS
#define LOGGING_SERIAL_WARNINGS
#define LOGGING_SERIAL_ERRORS
#define LOGGING_SERIAL_DEBUGS
#define LOGGING_CLASS_NAMES
#define SD_SELECT_PIN 53

#define MILLIS_PER_SECOND 1000UL
#define MILLIS_PER_MINUTE (MILLIS_PER_SECOND * 60)
#define MILLIS_PER_HOUR (MILLIS_PER_MINUTE * 60)

class Logger {

private:
    String className;

    String constructString(const String &s, const String &type);

public:

    Logger(String className);

    Logger(char *className);

    boolean logBare(const String &s);

    boolean log(const String &s);

    boolean log(char x);

    boolean log(int x);

    boolean log(byte x);

    boolean log(long x);

    boolean log(double x);

    boolean err(const String &s);

    boolean err(char x);

    boolean err(int x);

    boolean err(byte x);

    boolean err(long x);

    boolean err(double x);

    boolean warn(const String &s);

    boolean warn(char x);

    boolean warn(int x);

    boolean warn(byte x);

    boolean warn(long x);

    boolean warn(double x);

    boolean debug(const String &s);

    boolean debug(char x);

    boolean debug(int x);

    boolean debug(byte x);

    boolean debug(long x);

    boolean debug(double x);


private:
    static File logFile;
    static boolean initSuccess;

public:
    static boolean initialize();

    static String getTimeString();

};


#endif //ARDUINO_CODE_LOGGER_H
