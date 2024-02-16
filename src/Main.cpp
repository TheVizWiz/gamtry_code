#include "Arduino.h"
#include "AccelStepper.h"
#include "MultiStepper.h"
#include "Position.h"
#include "Gantry.h"

MultiStepper multiStepper();




void setup() {
    Serial.begin(115200);

}

void loop() {
    String message = Serial.readString();
}



