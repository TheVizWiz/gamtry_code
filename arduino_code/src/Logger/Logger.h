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
#define LOGGING true
#define LOGGING_TO_SERIAL true
#define LOGGING_SD_LOGS true
#define LOGGING_SD_WARNINGS true
#define LOGGING_SD_ERRORS true
#define LOGGING_SD_DEBUGS true
#define LOGGING_SERIAL_LOGS true
#define LOGGING_SERIAL_WARNINGS true
#define LOGGING_SERIAL_ERRORS true
#define LOGGING_SERIAL_DEBUGS true
#define SD_SELECT_PIN 53

#define MILLIS_PER_SECOND 1000UL
#define MILLIS_PER_MINUTE (MILLIS_PER_SECOND * 60)
#define MILLIS_PER_HOUR (MILLIS_PER_MINUTE * 60)

#define MAX_MESSAGE_LENGTH 80
#define MAX_TOTAL_MESSAGE_LENGTH 120

class Logger {

private:
    char className[20];
    char message[MAX_TOTAL_MESSAGE_LENGTH];
    char formatBuffer[120];

    char *constructLogMessage(const char *s, const char *type);

public:
    Logger(char *className);

    boolean logBare(const char *format, ...);


    boolean log(const char *format, ...);
    boolean log(const __FlashStringHelper *format, ...);


    boolean err(const char *format, ...);
    boolean err(const __FlashStringHelper *format, ...);


    boolean warn(const char *format, ...);
    boolean warn(const __FlashStringHelper *format, ...);


    boolean debug(const char *format, ...);
    boolean debug(const __FlashStringHelper *format, ...);





private:
    static File logFile;
    static boolean initSuccess;
    boolean vlog(const char *format, va_list args);
    boolean verr(const char *format, va_list args);
    boolean vwarn(const char *format, va_list args);
    boolean vdebug(const char *format, va_list args);

public:
    static boolean initialize();

    static char * getTimeString(char *buffer);

};


#endif //ARDUINO_CODE_LOGGER_H
