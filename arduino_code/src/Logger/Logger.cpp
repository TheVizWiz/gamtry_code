//
// Created by thevizwiz on 3/17/2024.
//

#include "Logger.h"


static char *flashToString(char *buffer, const __FlashStringHelper *ifsh) {
    size_t numLetters = 1;
    PGM_P p = reinterpret_cast<PGM_P>(ifsh);
    while (1) {
        unsigned char c = pgm_read_byte(p++);
        if (c == 0) break;
        numLetters++;
    }


    for (int i = 0; i < numLetters; i++) {
        buffer[i] = pgm_read_byte(p + i);
    }

    return buffer;
}

static Logger logger = Logger("Logger");

File Logger::logFile;

boolean Logger::initSuccess = false;

boolean Logger::initialize() {

    logger.log("Trying to open SD card...");

    pinMode(SD_SELECT_PIN, OUTPUT);

    if (!SD.begin(SD_SELECT_PIN)) {
        logger.log("No SD card found.");
        return false;
    }

    if (!SD.exists(LOGGING_DIR)) {
        logger.log("No logging directory exists. Making with name %s", LOGGING_DIR);
        SD.mkdir(LOGGING_DIR);
        logger.log("made directory. Exists: " + SD.exists(LOGGING_DIR));
    }

    File directory = SD.open(LOGGING_DIR);

    if (!directory)
        return false;
    if (!directory.isDirectory())
        return false;

    int numFiles = 0;

    //find number of files currently in the directory.
    // The name of the new file will be that number + 1.
    while (true) {
        File file = directory.openNextFile();
        if (!file)
            break;
        numFiles++;
    }

    char logFileName[100];


    sprintf(logFileName, "%s/%s%d%s", LOGGING_DIR, LOGGING_BASE_NAME, numFiles + 1, LOGGING_FILE_EXT);

    logger.log("New log file with name %s created.", logFileName);

    logFile = SD.open(logFileName, FILE_WRITE);
    if (!logFile)
        return false;

    logger.log("Successfully opened log file.");
    initSuccess = true;
    return true;
}

char *Logger::getTimeString(char *buffer) {

    uint32_t time = millis();

    uint32_t hrs = time / (MILLIS_PER_HOUR);
    time = time % (MILLIS_PER_HOUR);
    uint32_t mins = time / (MILLIS_PER_MINUTE);
    time = time % (MILLIS_PER_MINUTE);
    uint32_t secs = time / (MILLIS_PER_SECOND);
    uint32_t millis = time % (MILLIS_PER_SECOND);



    sprintf(buffer, "%02lu:%02lu:%02lu:%03lu", hrs, mins, secs, millis);

    return buffer;

}

char *Logger::constructLogMessage(const char *s, const char *type) {

    char timeString[50];

    getTimeString(timeString);

    sprintf(message, "[%s] (%s) {%s} %s",
            timeString,
            type,
            this->className,
            s);

    return message;
}


boolean Logger::logBare(const char *format, ...) {
    if (!LOGGING)
        return false;

    char input[MAX_TOTAL_MESSAGE_LENGTH];

    va_list args;
    va_start(args, format);

    vsprintf(input, format, args);


    if (LOGGING_SD_LOGS)
        logFile.println(input);

    if (LOGGING_TO_SERIAL && LOGGING_SERIAL_LOGS)
        Serial.println(input);

    return true;

}


boolean Logger::log(const char *format, ...) {

    if (!LOGGING)
        return false;


    char input[MAX_TOTAL_MESSAGE_LENGTH];

    va_list args;
    va_start(args, format);


    vsprintf(input, format, args);

    char *message = constructLogMessage(input, "LOG");


    if (LOGGING_SD_LOGS)
        logFile.println(message);

    if (LOGGING_TO_SERIAL && LOGGING_SERIAL_LOGS)
        Serial.println(message);

    free(message);
    return true;

}

boolean Logger::warn(const char *format, ...) {

    if (!LOGGING)
        return false;


    char input[MAX_TOTAL_MESSAGE_LENGTH];

    va_list args;
    va_start(args, format);

    vsprintf(input, format, args);

    constructLogMessage(input, "WAR");


    if (LOGGING_SD_WARNINGS)
        logFile.println(message);

    if (LOGGING_TO_SERIAL && LOGGING_SERIAL_WARNINGS)
        Serial.println(message);

    free(message);
    return true;

}

boolean Logger::err(const char *format, ...) {

    if (!LOGGING)
        return false;


    char input[MAX_TOTAL_MESSAGE_LENGTH];

    va_list args;
    va_start(args, format);

    vsprintf(input, format, args);

    char *message = constructLogMessage(input, "ERR");


    if (LOGGING_SD_ERRORS)
        logFile.println(message);

    if (LOGGING_TO_SERIAL && LOGGING_SERIAL_ERRORS)
        Serial.println(message);

    free(message);
    return true;

}

boolean Logger::debug(const char *format, ...) {

    if (!LOGGING)
        return false;


    char input[MAX_TOTAL_MESSAGE_LENGTH];

    va_list args;
    va_start(args, format);

    vsprintf(input, format, args);

    char *message = constructLogMessage(input, "DEB");


    if (LOGGING_SD_DEBUGS)
        logFile.println(message);

    if (LOGGING_TO_SERIAL && LOGGING_SERIAL_DEBUGS)
        Serial.println(message);

    free(message);
    return true;

}


Logger::Logger(char *className) {
    strcpy(this->className, className);
}

boolean Logger::log(const __FlashStringHelper *format, ...) {
    va_list args;
    va_start(args, format);

    char *stringFormat = flashToString(formatBuffer, format);

    boolean success = vlog(stringFormat, args);
    return success;
}

boolean Logger::err(const __FlashStringHelper *format, ...) {
    va_list args;
    va_start(args, format);

    char *stringFormat = flashToString(formatBuffer, format);

    boolean success = verr(stringFormat, args);
    free(stringFormat);
    return success;
}

boolean Logger::warn(const __FlashStringHelper *format, ...) {
    va_list args;
    va_start(args, format);

    char *stringFormat = flashToString(formatBuffer, format);

    boolean success = vwarn(stringFormat, args);
    free(stringFormat);
    return success;
}

boolean Logger::debug(const __FlashStringHelper *format, ...) {
    va_list args;
    va_start(args, format);

    char *stringFormat = flashToString(formatBuffer, format);

    boolean success = vdebug(stringFormat, args);
    free(stringFormat);
    return success;
}

boolean Logger::vlog(const char *format, va_list args) {

    if (!LOGGING)
        return false;


    char input[MAX_TOTAL_MESSAGE_LENGTH];


    vsprintf(input, format, args);

    char *message = constructLogMessage(input, "LOG");


    if (LOGGING_SD_LOGS)
        logFile.println(message);

    if (LOGGING_TO_SERIAL && LOGGING_SERIAL_LOGS)
        Serial.println(message);

    free(message);
    return true;
}

boolean Logger::verr(const char *format, va_list args) {

    if (!LOGGING)
        return false;


    char input[MAX_TOTAL_MESSAGE_LENGTH];


    vsprintf(input, format, args);

    char *message = constructLogMessage(input, "ERR");


    if (LOGGING_SD_LOGS)
        logFile.println(message);

    if (LOGGING_TO_SERIAL && LOGGING_SERIAL_LOGS)
        Serial.println(message);

    free(message);
    return true;
}

boolean Logger::vwarn(const char *format, va_list args)     {

    if (!LOGGING)
        return false;


    char input[MAX_TOTAL_MESSAGE_LENGTH];


    vsprintf(input, format, args);

    char *message = constructLogMessage(input, "WAR");


    if (LOGGING_SD_LOGS)
        logFile.println(message);

    if (LOGGING_TO_SERIAL && LOGGING_SERIAL_LOGS)
        Serial.println(message);

    free(message);

    return true;
}

boolean Logger::vdebug(const char *format, va_list args) {

    if (!LOGGING)
        return false;


    char input[MAX_TOTAL_MESSAGE_LENGTH];


    vsprintf(input, format, args);

    char *message = constructLogMessage(input, "LOG");


    if (LOGGING_SD_LOGS)
        logFile.println(message);

    if (LOGGING_TO_SERIAL && LOGGING_SERIAL_LOGS)
        Serial.println(message);

    free(message);

    return true;
}




