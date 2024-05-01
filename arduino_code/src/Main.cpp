#include "Arduino.h"
#include "Command/CommandQueue.h"
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

//
    Serial.begin(115200);
    Serial.setTimeout(10);
    Serial1.begin(115200);
    Serial1.setTimeout(10);
    Logger::initialize();
    gantry.initialize();

//    logger.debug("writing to servo");
//
//    Servo servo = Servo();
//    servo.attach(10);
//    servo.write(0);
//    delay(1000);
//    servo.write(90);
//    delay(1000);
//    servo.write(180);
//    delay(1000);
//    servo.write(90);
//    delay(1000);
//    servo.write(0);
//
//    logger.debug("finished writing to servo");


//
//
//
//
//
//
//    CommandParser::parse("S0").execute(gantry);
//    delay(1000);
//


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
//    Test::testLimitSwitches(gantry);
//    return;




    queue.queueCommand();
    Command command = queue.popNextCommand();
//
//
    if (command.isNoCommand())
        return;

    Serial.println("Running");
    Serial1.println("Running");

    command.execute(gantry);
    Serial.println("Done"); 
    Serial1.println("Done"); 

}
