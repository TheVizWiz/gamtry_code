#include "Arduino.h"
#include "AccelStepper.h"
#include "Command/CommandQueue.h"
#include "Command/CommandParser.h"

//
CommandQueue queue = CommandQueue();
GantryConfiguration gantry = GantryConfiguration();


#define STEPS_PER_SECOND 180
#define RANGE 100
#define TIME (RANGE / (X_MM_PER_STEP * STEPS_PER_SECOND))
//
//AccelStepper stepper1 = AccelStepper(AccelStepper::DRIVER, 5, 6);
//AccelStepper stepper2 = AccelStepper(AccelStepper::DRIVER, 7, 8);


void setup() {
    Serial.begin(115200);
    Serial.setTimeout(10);
    Serial1.begin(115200);
    Serial1.setTimeout(10);
//
//    queue.commands.push_back(CommandParser::parse("X40"));
//    queue.commands.push_back(CommandParser::parse("X0"));
//    queue.commands.push_back(CommandParser::parse("X80"));
//    queue.commands.push_back(CommandParser::parse("X0"));
//    queue.commands.push_back(CommandParser::parse("X120"));
//    queue.commands.push_back(CommandParser::parse("X0"));
//    queue.commands.push_back(CommandParser::parse("X160"));
//    queue.commands.push_back(CommandParser::parse("X0"));
//    queue.commands.push_back(CommandParser::parse("X200"));



}

void loop() {


//
//    stepper.setSpeed(180);
//    for (int i = 0; i < 1000; i++) {
//        stepper.runSpeed();
//        delay(1);
//    }
//    for (int i = 0; i < 180; i++) {
//        digitalWrite(STEP, HIGH);
//        delayMicroseconds(2000);
//        digitalWrite(STEP, LOW);
//        delayMicroseconds(2000);
//    }

//    digitalWrite(LED_BUILTIN, HIGH);
//
//    Serial.println("going one way");
//
//    stepper1.setSpeed(STEPS_PER_SECOND);
//    stepper2.setSpeed(STEPS_PER_SECOND);
//    unsigned long currentTime = millis();
//    while (millis() - currentTime < TIME * 1000) {
//        stepper1.runSpeed();
//        stepper2.runSpeed();
//    }
//
//    Serial.print(String("after one: ") + stepper1.currentPosition() + " " + stepper2.currentPosition());
//
//    delay(500);
//
//
//    while(true);
//
//
//    Serial.println("going other way");
//    digitalWrite(LED_BUILTIN, LOW);
//    stepper1.setSpeed(-STEPS_PER_SECOND);
//    stepper2.setSpeed(-STEPS_PER_SECOND);
//    currentTime = millis();
//    while (millis() - currentTime < TIME * 1000) {
//        stepper1.runSpeed();
//        stepper2.runSpeed();
//    }
//    Serial.print(String("after other: ") + stepper1.currentPosition() + " " + stepper2.currentPosition());
//    delay(500);

    queue.queueCommand();
    Command command = queue.popNextCommand();


    if (command.isNoCommand())
        return;


//    Serial.println(command.toString());

    command.execute(gantry);
//
    Serial.println("X POSITION:");
    Serial.println(gantry.position.x);

}



