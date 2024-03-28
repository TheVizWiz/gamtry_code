#include "Arduino.h"
#include "Command/CommandQueue.h"
#include "Command/CommandParser.h"
#include "Command/Command.h"
#include "Test/Test.h"
#include "Logger/Logger.h"

//
CommandQueue queue = CommandQueue();
GantryConfiguration gantry = GantryConfiguration();


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
    delay(5000);
//    gantry.homeZAxis();
//    CommandParser::parse("Z90").execute(gantry);
//    gantry.homeXAxis();
//    gantry.homeYAxis();
//    delay(2000);
//    CommandParser::parse("X443 Y214").execute(gantry);
//    CommandParser::parse(("Z75")).execute(gantry);


//    CommandParser::parse("S0").execute(gantry);
//    CommandParser::parse("X200 Y400 Z20").execute(gantry);
//    delay(5000);
//
//
//    CommandParser::parse("Z75").execute(gantry);
//    CommandParser::parse("X391.5 Y210").execute(gantry);
//    CommandParser::parse("Z37.5").execute(gantry);
//    CommandParser::parse("Z42").execute(gantry);
//    CommandParser::parse("X300").execute(gantry);
//    CommandParser::parse("X200 Y400 Z20").execute(gantry);
//
//    delay(5000);
//
//    CommandParser::parse("Z42").execute(gantry);
//    CommandParser::parse("X350 Y210").execute(gantry);
//    CommandParser::parse("X392").execute(gantry);
//    CommandParser::parse("Z27").execute(gantry);
//    CommandParser::parse("X330").execute(gantry);
//    CommandParser::parse("X200 Y400 Z20").execute(gantry);

//    REMOVAL SEQUENCE
//    Z75
//    X394 Y206
//    Z37.5
//    Z42
//    X350
//
//
//    TRAY DOCKING SEQUENCE
//    Z42
//    X350 Y206
//    X395 t2
//    Z27
//    X330 t2
}

void loop() {
    Test::testLimitSwitches(gantry);
    return;




    queue.queueCommand();
    Command command = queue.popNextCommand();
//
//
    if (command.isNoCommand())
        return;


    command.execute(gantry);


}



