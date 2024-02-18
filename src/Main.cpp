#include "Arduino.h"
#include "AccelStepper.h"
#include "MultiStepper.h"
#include "Position.h"
#include "Gantry.h"
#include "Vector.h"
#include "Command.h"
#include "Servo.h"


CommandQueue queue = CommandQueue();
Servo servo = Servo();
long numCommands = 0;


void setup() {
    Serial.begin(115200);
    Serial.setTimeout(10);
    servo.attach(12);
}

void loop() {

    queue.queueCommand();
    Command command = queue.popNextCommand();

    if (command.isNoCommand)
        return;


    Serial.println(command.toString());

    if (command.theta_changed)
        servo.write(command.theta);

}



