#include "Arduino.h"
#include "Command/CommandQueue.h"
#include "Command/CommandParser.h"
#include "Command/Command.h"
#include "Test/Test.h"
#include "Logger/Logger.h"

//
CommandQueue queue;
GantryConfiguration gantry;


#define STEPS_PER_SECOND 180
#define RANGE 100
#define TIME (RANGE / (X_MM_PER_STEP * STEPS_PER_SECOND))
//
//AccelStepper stepper1 = AccelStepper(AccelStepper::DRIVER, 5, 6);
//AccelStepper stepper2 = AccelStepper(AccelStepper::DRIVER, 7, 8);

Logger logger = "Main";


void setup() {

    Serial.begin(115200);
    Serial.setTimeout(10);
    Serial1.begin(115200);
    Serial1.setTimeout(10);
    Logger::initialize();
    gantry.initialize();


    gantry = GantryConfiguration();
    queue = CommandQueue();


    for (int i = 0; i < 10; i++) {
        logger.log(i);
        logger.warn(i * PI);
        logger.err(i + String("test"))  ;
    }




//
//
//    queue.commands.push_back(CommandParser::parse("X0"));
//    queue.commands.push_back(CommandParser::parse("X100"));
//    queue.commands.push_back(CommandParser::parse("X0"));
//    queue.commands.push_back(CommandParser::parse("X200"));
//    queue.commands.push_back(CommandParser::parse("X0"));
//    queue.commands.push_back(CommandParser::parse("X-100"));
//    queue.commands.push_back(CommandParser::parse("X0"));
//    queue.commands.push_back(CommandParser::parse("X-200"));
//    queue.commands.push_back(CommandParser::parse("X0"));
//
//    queue.commands.push_back(CommandParser::parse("T0"));
//    queue.commands.push_back(CommandParser::parse("T45"));
//    queue.commands.push_back(CommandParser::parse("T0"));
//    queue.commands.push_back(CommandParser::parse("T90"));
//    queue.commands.push_back(CommandParser::parse("T0"));
//    queue.commands.push_back(CommandParser::parse("T-45"));
//    queue.commands.push_back(CommandParser::parse("T0"));
//    queue.commands.push_back(CommandParser::parse("T-90"));
//    queue.commands.push_back(CommandParser::parse("T0"));
//
//    queue.commands.push_back(CommandParser::parse("T0 X0"));
//    queue.commands.push_back(CommandParser::parse("T45 X100"));
//    queue.commands.push_back(CommandParser::parse("T0 X0"));
//    queue.commands.push_back(CommandParser::parse("T90 X200"));
//    queue.commands.push_back(CommandParser::parse("T0 X0"));
//    queue.commands.push_back(CommandParser::parse("T-45 X-100"));
//    queue.commands.push_back(CommandParser::parse("T0 X0"));
//    queue.commands.push_back(CommandParser::parse("T-90 X-200"));
//    queue.commands.push_back(CommandParser::parse("T0 X0"));

}

void loop() {
//    Test::testLimitSwitches(gantry);
//    return;




    queue.queueCommand();
    Command command = queue.popNextCommand();
//
//
    if (command.isNoCommand())
        return;


    command.execute(gantry);


}



