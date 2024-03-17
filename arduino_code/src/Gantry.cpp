//
// Created by thevizwiz on 2/29/2024.
//



#include "Gantry.h"

GantryConfiguration::GantryConfiguration() {

    head = Head();


    x1_motor = AccelStepper(AccelStepper::DRIVER, X1_MOTOR_PINS);
    x2_motor = AccelStepper(AccelStepper::DRIVER, X2_MOTOR_PINS);
    y_motor = AccelStepper(AccelStepper::DRIVER, Y_MOTOR_PINS);
    z_motor = AccelStepper(AccelStepper::DRIVER, Z_MOTOR_PINS);
    theta_motor = AccelStepper(AccelStepper::DRIVER, THETA_MOTOR_PINS);


    x1_motor.setMaxSpeed(X_MAX_STEPS_PER_SECOND);
    x2_motor.setMaxSpeed(X_MAX_STEPS_PER_SECOND);
    y_motor.setMaxSpeed(Y_MAX_STEPS_PER_SECOND);
    z_motor.setMaxSpeed(Z_MAX_STEPS_PER_SECOND);
    theta_motor.setMaxSpeed(THETA_MAX_STEPS_PER_SECOND);

    if (LIMIT_SWITCH_NORMALLY_CLOSED) {
        pinMode(X1_LIMIT_SWITCH_PIN, INPUT_PULLUP);
        pinMode(X2_LIMIT_SWITCH_PIN, INPUT_PULLUP);
        pinMode(Y_LIMIT_SWITCH_PIN, INPUT_PULLUP);
        pinMode(Z_LIMIT_SWITCH_PIN, INPUT_PULLUP);
        pinMode(THETA_LIMIT_SWITCH_PIN, INPUT_PULLUP);
    } else {

        //note: assumes external pulldown resister. No resistor is needed for pullup, only for pulldown. If no external pulldown resistor, better to just use normally closed with ground.
        pinMode(X1_LIMIT_SWITCH_PIN, INPUT);
        pinMode(X2_LIMIT_SWITCH_PIN, INPUT);
        pinMode(Y_LIMIT_SWITCH_PIN, INPUT);
        pinMode(Z_LIMIT_SWITCH_PIN, INPUT);
        pinMode(THETA_LIMIT_SWITCH_PIN, INPUT);
    }


}

void GantryConfiguration::updatePosition() {
    position.x = (x1_motor.currentPosition() + x2_motor.currentPosition()) / 2 * X_MM_PER_STEP;
    position.y = y_motor.currentPosition() * Y_MM_PER_STEP;
    position.z = z_motor.currentPosition() * Z_MM_PER_STEP;
    position.theta = theta_motor.currentPosition() * THETA_DEG_PER_STEP;
}

void GantryConfiguration::homeXAxis() {

    Serial.println(String("Moving X Axis at ") + X_ZEROING_STEPS_PER_SECOND + " steps per second");
    x1_motor.setSpeed(-X_ZEROING_STEPS_PER_SECOND);
    x2_motor.setSpeed(-X_ZEROING_STEPS_PER_SECOND);
    while (!x1LimitSwitchTriggered() ||
           !x2LimitSwitchTriggered()) {
        if (!x1LimitSwitchTriggered())
            x1_motor.runSpeed();
        if (!x2LimitSwitchTriggered())
            x2_motor.runSpeed();
    }


    Serial.println("HIT X AXIS SWITCH. Moving X Axis back...");
    x1_motor.setSpeed(X_ZEROING_STEPS_PER_SECOND);
    x2_motor.setSpeed(X_ZEROING_STEPS_PER_SECOND);
    unsigned long currentTime = millis();
    while (millis() <= currentTime + 1000) {
        x1_motor.runSpeed();
        x2_motor.runSpeed();
    }

    Serial.println("Moving X Axis back at half speed...");
    x1_motor.setSpeed(-X_ZEROING_STEPS_PER_SECOND / 2);
    x2_motor.setSpeed(-X_ZEROING_STEPS_PER_SECOND / 2);
    while (!x1LimitSwitchTriggered() ||
           !x2LimitSwitchTriggered()) {
        if (!x1LimitSwitchTriggered())
            x1_motor.runSpeed();
        if (!x2LimitSwitchTriggered())
            x2_motor.runSpeed();
    }

    Serial.println("HIT X AXIS SWITCH AGAIN. Setting as 0.");
    x1_motor.setCurrentPosition(0);
    x2_motor.setCurrentPosition(0);
    position.x = 0;
}

void GantryConfiguration::homeYAxis() {


    Serial.println(String("Moving Y Axis at ") + Y_ZEROING_STEPS_PER_SECOND + " steps per second");
    y_motor.setSpeed(-Y_ZEROING_STEPS_PER_SECOND);
    while (!yLimitSwitchTriggered()) {
        y_motor.runSpeed();
    }


    Serial.println("HIT Y AXIS SWITCH. Moving Y Axis back...");
    y_motor.setSpeed(Y_ZEROING_STEPS_PER_SECOND);
    unsigned long currentTime = millis();
    while (millis() <= currentTime + 1000) {
        y_motor.runSpeed();
    }

    Serial.println("Moving Y Axis back at half speed...");
    y_motor.setSpeed(-Y_ZEROING_STEPS_PER_SECOND / 2);
    while (!yLimitSwitchTriggered()) {
        y_motor.runSpeed();
    }

    Serial.println("HIT Y AXIS SWITCH AGAIN. Setting as 0.");
    y_motor.setCurrentPosition(0);
    position.y = 0;
}

void GantryConfiguration::homeZAxis() {


    Serial.println(String("Moving Z Axis at ") + Z_ZEROING_STEPS_PER_SECOND + " steps per second");
    z_motor.setSpeed(-Z_ZEROING_STEPS_PER_SECOND);
    while (!zLimitSwitchTriggered()) {
        z_motor.runSpeed();
    }

    Serial.println("HIT Z AXIS SWITCH. Moving Z Axis back...");
    z_motor.setSpeed(Z_ZEROING_STEPS_PER_SECOND);
    unsigned long currentTime = millis();
    while (millis() <= currentTime + 1000) {
        z_motor.runSpeed();
    }

    Serial.println("Moving Z Axis back at half speed...");
    z_motor.setSpeed(-Z_ZEROING_STEPS_PER_SECOND / 2);
    while (!zLimitSwitchTriggered()) {
        z_motor.runSpeed();
    }

    Serial.println("HIT Z AXIS SWITCH AGAIN. Setting as 0.");
    z_motor.setCurrentPosition(0);
    position.z = 0;


}

void GantryConfiguration::homeThetaAxis() {


    Serial.println(String("Moving Theta Axis at ") + THETA_ZEROING_STEPS_PER_SECOND + " steps per second");
    theta_motor.setSpeed(-THETA_ZEROING_STEPS_PER_SECOND);
    while (!thetaLimitSwitchTriggered()) {
        theta_motor.runSpeed();
    }

    Serial.println("HIT Theta AXIS SWITCH. Moving Theta Axis back...");
    theta_motor.setSpeed(THETA_ZEROING_STEPS_PER_SECOND);
    unsigned long currentTime = millis();
    while (millis() <= currentTime + 1000) {
        theta_motor.runSpeed();
    }

    Serial.println("Moving Theta Axis back at half speed...");
    theta_motor.setSpeed(-THETA_ZEROING_STEPS_PER_SECOND / 2);
    while (!thetaLimitSwitchTriggered()) {
        theta_motor.runSpeed();
    }

    Serial.println("HIT Theta AXIS SWITCH AGAIN. Setting as 0.");
    theta_motor.setCurrentPosition(0);
    position.theta = 0;

}

boolean GantryConfiguration::x1LimitSwitchTriggered() {
    return !digitalRead(X1_LIMIT_SWITCH_PIN);
}

boolean GantryConfiguration::x2LimitSwitchTriggered() {
    return !digitalRead(X2_LIMIT_SWITCH_PIN);
}

boolean GantryConfiguration::yLimitSwitchTriggered() {
    return !digitalRead(Y_LIMIT_SWITCH_PIN);
}

boolean GantryConfiguration::zLimitSwitchTriggered() {
    return !digitalRead(Z_LIMIT_SWITCH_PIN);
}

boolean GantryConfiguration::thetaLimitSwitchTriggered() {
    return !digitalRead(THETA_LIMIT_SWITCH_PIN);
}

boolean GantryConfiguration::xMaxLimitReached() {
    return position.x >= MAX_X_MM;
}

boolean GantryConfiguration::yMaxLimitReached() {
    return position.y >= MAX_Y_MM;
}

boolean GantryConfiguration::zMaxLimitReached() {
    return position.z >= MAX_Z_MM;
}

boolean GantryConfiguration::thetaMaxLimitReached() {
    return position.theta >= MAX_THETA_DEG;
}

