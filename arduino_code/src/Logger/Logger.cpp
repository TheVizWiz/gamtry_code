//
// Created by thevizwiz on 3/17/2024.
//

#include "Logger.h"


static Logger logger = Logger("Logger");

File Logger::logFile = SD.open("log.txt");

boolean Logger::initSuccess = false;

boolean Logger::initialize() {
    if (!SD.begin(SD_SELECT_PIN))
        return false;

    if (!SD.exists(LOGGING_DIR))
        SD.mkdir(LOGGING_DIR);

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

    String logFileName = String() +
                         LOGGING_DIR +
                         "/" +
                         LOGGING_BASE_NAME
                         + (numFiles + 1)
                         + LOGGING_FILE_EXT;

    logFile = SD.open(logFileName, FILE_WRITE);
    if (!logFile)
        return false;

    logger.log("Successfully opened log file.");
    initSuccess = true;
    return true;
}

String Logger::getTimeString() {

    uint32_t time = millis();

    uint32_t hrs = time / (MILLIS_PER_HOUR);
    time = time % (MILLIS_PER_HOUR);
    uint32_t mins = time / (MILLIS_PER_MINUTE);
    time = time % (MILLIS_PER_MINUTE);
    uint32_t secs = time / (MILLIS_PER_SECOND);
    uint32_t millis = time % (MILLIS_PER_SECOND);

    String hoursString = String(hrs);
    String minsString = String(mins);
    String secsString = String(secs);
    String millisString = String(millis);

    while (hoursString.length() < 2)
        hoursString = '0' + hoursString;
    while (minsString.length() < 2)
        minsString = '0' + minsString;
    while (secsString.length() < 2)
        secsString = '0' + secsString;
    while (millisString.length() < 3)
        millisString = millisString + '0';

    String timeString = hoursString + ":" +
                        minsString + ":" +
                        secsString + "." + millisString;

    return timeString;

}

String Logger::constructString(const String &s, const String &type) {
    String message = "";
#ifdef LOGGING_TIMESTAMPS
    message = message + "[" + getTimeString() + "] ";
#endif

    message = message + "(" + type + ") ";

#ifdef LOGGING_CLASS_NAMES
    message = message + "{" + className + "} ";
#endif

    message += s;

    return message;
}


boolean Logger::logBare(const String &s) {
#ifndef LOGGING
    return false;
#endif

    String message = s;
#ifndef LOGGING_SD_LOGS
    logFile.println(message);
#endif

#ifndef LOGGING_TO_SERIAL
    return true;
#endif

#ifndef LOGGING_SERIAL_LOGS
    return true;
#endif

    Serial.println(message);
    return true;

}

boolean Logger::log(const String &s) {


#ifndef LOGGING
    return false;
#endif


    String message = constructString(s, "LOG");

#ifndef LOGGING_SD_LOGS
    logFile.println(message);
#endif

#ifndef LOGGING_TO_SERIAL
    return true;
#endif

#ifndef LOGGING_SERIAL_LOGS
    return true;
#endif

    Serial.println(message);
    return true;
}

boolean Logger::log(char x) {
    return log(String(x));
}

boolean Logger::log(int x) {
    return log(String(x));
}

boolean Logger::log(byte x) {
    return log(String(x));
}

boolean Logger::log(long x) {
    return log(String(x));
}

boolean Logger::log(double x) {
    return log(String(x));
}

boolean Logger::warn(const String &s) {
#ifndef LOGGING
    return false;
#endif


    String message = constructString(s, "WAR");

#ifndef LOGGING_SD_WARNINGS
    logFile.println(message);
#endif

#ifndef LOGGING_TO_SERIAL
    return true;
#endif

#ifndef LOGGING_SERIAL_WARNINGS
    return true;
#endif

    Serial.println(message);
    return true;
}

boolean Logger::warn(char x) {
    return warn(String(x));
}

boolean Logger::warn(int x) {
    return warn(String(x));
}

boolean Logger::warn(byte x) {
    return warn(String(x));
}

boolean Logger::warn(long x) {
    return warn(String(x));
}

boolean Logger::warn(double x) {
    return warn(String(x));
}

boolean Logger::err(const String &s) {
#ifndef LOGGING
    return false;
#endif


    String message = constructString(s, "ERR");

#ifndef LOGGING_SD_ERRORS
    logFile.println(message);
#endif

#ifndef LOGGING_TO_SERIAL
    return true;
#endif

#ifndef LOGGING_SERIAL_ERRORS
    return true;
#endif

    Serial.println(message);
    return true;


}

boolean Logger::err(char x) {
    return err(String(x));
}

boolean Logger::err(int x) {
    return err(String(x));
}

boolean Logger::err(byte x) {
    return err(String(x));
}

boolean Logger::err(long x) {
    return err(String(x));
}

boolean Logger::err(double x) {
    return err(String(x));
}

boolean Logger::debug(const String &s) {
#ifndef LOGGING
    return false;
#endif


    String message = constructString(s, "DEB");

#ifndef LOGGING_SD_DEBUGS
    logFile.println(message);
#endif

#ifndef LOGGING_TO_SERIAL
    return true;
#endif

#ifndef LOGGING_SERIAL_DEBUGS
    return true;
#endif

    Serial.println(message);
    return true;


}

boolean Logger::debug(char x) {
    return debug(String(x));
}

boolean Logger::debug(int x) {
    return debug(String(x));
}

boolean Logger::debug(byte x) {
    return debug(String(x));
}

boolean Logger::debug(long x) {
    return debug(String(x));
}

boolean Logger::debug(double x) {
    return debug(String(x));
}

Logger::Logger(String className) {
    this->className = className.c_str();
}

Logger::Logger(char *className) : Logger(String(className)) {}




