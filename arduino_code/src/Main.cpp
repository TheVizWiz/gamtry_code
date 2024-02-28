#include "Arduino.h"
#include "AccelStepper.h"
#include "MultiStepper.h"
#include "Position.h"
#include "Gantry.h"
#include "Vector.h"
#include "Command/Command.h"
#include "Servo.h"
#include "Command/CommandQueue.h"


CommandQueue queue = CommandQueue();
Servo servo = Servo();
long numCommands = 0;


GantryConfiguration gantry = GantryConfiguration();


void setup() {
    Serial.begin(115200);
    Serial.setTimeout(10);
    servo.attach(12);
}

void loop() {

    queue.queueCommand();
    Command command = queue.popNextCommand();


    if (command.isNoCommand())
        return;


    command.execute(gantry);

}



