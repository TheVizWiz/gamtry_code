//



#include "Gantry.h"
#include "Logger/Logger.h"
#include "Command/CommandParser.h"

static Logger logger = Logger("Gantry");


static volatile boolean x1LimitSwitch,
        x2LimitSwitch,
        yLimitSwitch,
        zLimitSwitch,
        thetaLimitSwitch;

static void clickX1();

static void clickX2();

static void clickY();

static void clickZ();

static void clickTheta();


//
// Created by thevizwiz on 2/29/2024.
boolean GantryConfiguration::initialize() {

    logger.log(F("initializing gantry"));

    logger.log(F("initializing motors x1, x2, y, z, theta"));
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


    x1_motor.setAcceleration(X_ACCELERATION);
    x2_motor.setAcceleration(X_ACCELERATION);
    y_motor.setAcceleration(Y_ACCELERATION);
    z_motor.setAcceleration(Z_ACCELERATION);
    theta_motor.setAcceleration(THETA_ACCELERATION);


    logger.log(F("limit switch configuration: normally closed. 0 if closed, 1 if open"));
    pinMode(X1_LIMIT_SWITCH_PIN, INPUT_PULLUP);
    pinMode(X2_LIMIT_SWITCH_PIN, INPUT_PULLUP);
    pinMode(Y_LIMIT_SWITCH_PIN, INPUT_PULLUP);
    pinMode(Z_LIMIT_SWITCH_PIN, INPUT_PULLUP);
    pinMode(THETA_LIMIT_SWITCH_PIN, INPUT_PULLUP);

    
    gripperServo.attach(GRIPPER_PIN);
    gripperServo.write(0);

#ifdef LIMIT_SWITCH_INTERRUPTS
    attachInterrupt(digitalPinToInterrupt(X1_LIMIT_SWITCH_PIN), clickX1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(X2_LIMIT_SWITCH_PIN), clickX2, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Y_LIMIT_SWITCH_PIN), clickY, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Z_LIMIT_SWITCH_PIN), clickZ, CHANGE);
    attachInterrupt(digitalPinToInterrupt(THETA_LIMIT_SWITCH_PIN), clickTheta, CHANGE);
#endif
    return true;
}

void GantryConfiguration::updatePosition() {
    position.x = (x1_motor.currentPosition() + x2_motor.currentPosition()) / 2 * X_MM_PER_STEP;
    position.y = y_motor.currentPosition() * Y_MM_PER_STEP;
    position.z = z_motor.currentPosition() * Z_MM_PER_STEP;
    position.theta = theta_motor.currentPosition() * THETA_DEG_PER_STEP;
}

void GantryConfiguration::homeXAxis() {
    logger.log(F("Homing X Axis."));
    char str[100];
    sprintf(str, "Moving X Axis at %f steps per second", X_ZEROING_STEPS_PER_SECOND);
    logger.log(str);
    x1_motor.setSpeed(-X_ZEROING_STEPS_PER_SECOND);
    x2_motor.setSpeed(-X_ZEROING_STEPS_PER_SECOND);
    while (!x1LimitSwitchTriggered() ||
           !x2LimitSwitchTriggered()) {
        if (!x1LimitSwitchTriggered())
            x1_motor.runSpeed();
        if (!x2LimitSwitchTriggered())
            x2_motor.runSpeed();
    }


    logger.log(F("HIT X AXIS SWITCH. Moving X Axis back..."));
    x1_motor.setSpeed(X_ZEROING_STEPS_PER_SECOND);
    x2_motor.setSpeed(X_ZEROING_STEPS_PER_SECOND);
    unsigned long currentTime = millis();
    while (millis() <= currentTime + 1000) {
        x1_motor.runSpeed();
        x2_motor.runSpeed();
    }

    logger.log(F("Moving X Axis back..."));
    x1_motor.setSpeed(-X_ZEROING_STEPS_PER_SECOND / 2);
    x2_motor.setSpeed(-X_ZEROING_STEPS_PER_SECOND / 2);
    while (!x1LimitSwitchTriggered() ||
           !x2LimitSwitchTriggered()) {
        if (!x1LimitSwitchTriggered())
            x1_motor.runSpeed();
        if (!x2LimitSwitchTriggered())
            x2_motor.runSpeed();
    }




    logger.log(F("Hit X axis switch again. Setting to 0. Delta to old 0: %f, %f."),
               x1_motor.currentPosition(),
               x2_motor.currentPosition());
    x1_motor.setCurrentPosition(0);
    x2_motor.setCurrentPosition(0);
    position.x = 0;
}

void GantryConfiguration::homeYAxis() {

    logger.log(F("Homing Y Axis."));

    logger.log(F("Moving Y Axis at %f steps per second"),  Y_ZEROING_STEPS_PER_SECOND);
    y_motor.setSpeed(-Y_ZEROING_STEPS_PER_SECOND);
    while (!yLimitSwitchTriggered()) {
        y_motor.runSpeed();
    }


    logger.log(F("HIT Y AXIS SWITCH. Moving Y Axis back..."));
    y_motor.setSpeed(Y_ZEROING_STEPS_PER_SECOND);
    unsigned long currentTime = millis();
    while (millis() <= currentTime + 1000) {
        y_motor.runSpeed();
    }

    logger.log(F("Moving Y Axis back..."));
    y_motor.setSpeed(-Y_ZEROING_STEPS_PER_SECOND / 2);
    while (!yLimitSwitchTriggered()) {
        y_motor.runSpeed();
    }

    logger.log(F("Hit Y axis switch again. Setting to 0. Delta to old 0: %s."),
               y_motor.currentPosition());
    y_motor.setCurrentPosition(0);
    position.y = 0;
}

void GantryConfiguration::homeZAxis() {
    logger.log("Homing Z Axis.");


    logger.log(F("Moving Z Axis at %f steps per second"), Z_ZEROING_STEPS_PER_SECOND);
    z_motor.moveTo(-1000000000);
    z_motor.setSpeed(-Z_ZEROING_STEPS_PER_SECOND);
    while (!zLimitSwitchTriggered()) {
        z_motor.runSpeedToPosition();
    }

//    logger.log("HIT Z AXIS SWITCH. Moving Z Axis back...");
//    z_motor.setSpeed(Z_ZEROING_STEPS_PER_SECOND);
//    unsigned long currentTime = millis();
//    while (millis() <= currentTime + 2000) {
//        z_motor.runSpeed();
//    }
//
//    logger.log("Moving Z Axis back...");
//    z_motor.setSpeed(-Z_ZEROING_STEPS_PER_SECOND);
//    while (!zLimitSwitchTriggered()) {
//        z_motor.runSpeed();
//    }

    logger.log(F("Hit Z axis switch. Setting to 0. Delta to old 0: %d"), z_motor.currentPosition());

    z_motor.setCurrentPosition(0);
    position.z = 0;


}

void GantryConfiguration::homeThetaAxis() {

    logger.log("Homing Theta Axis.");

    logger.log(F("Moving Theta Axis at %f steps per second"), THETA_ZEROING_STEPS_PER_SECOND);
    theta_motor.setSpeed(-THETA_ZEROING_STEPS_PER_SECOND);
    while (!thetaLimitSwitchTriggered()) {
        theta_motor.runSpeed();
    }

    logger.log(F("HIT Theta AXIS SWITCH. Moving Theta Axis back..."));
    theta_motor.setSpeed(THETA_ZEROING_STEPS_PER_SECOND);
    unsigned long currentTime = millis();
    while (millis() <= currentTime + 1000) {
        theta_motor.runSpeed();
    }

    logger.log(F("Moving Theta Axis back..."));
    theta_motor.setSpeed(-THETA_ZEROING_STEPS_PER_SECOND);
    while (!thetaLimitSwitchTriggered()) {
        theta_motor.runSpeed();
    }

    logger.log(F("Hit Theta axis switch again. Setting to 0. Delta to old 0: %d"), theta_motor.currentPosition());
    theta_motor.setCurrentPosition(0);
    position.theta = 0;
    execute("T11.5");
    logger.log(F("Moving 11.5 degrees and setting as new 0 point aligned with axes."));
    theta_motor.setCurrentPosition(0);
    position.theta = 0;

}

boolean GantryConfiguration::x1LimitSwitchTriggered() {
#ifdef LIMIT_SWITCH_INTERRUPTS
    return x1LimitSwitch;
#else
    return digitalRead(X1_LIMIT_SWITCH_PIN);
#endif
}

boolean GantryConfiguration::x2LimitSwitchTriggered() {
#ifdef LIMIT_SWITCH_INTERRUPTS
    return x2LimitSwitch;
#else
    return digitalRead(X2_LIMIT_SWITCH_PIN);
#endif
}

boolean GantryConfiguration::yLimitSwitchTriggered() {
#ifdef LIMIT_SWITCH_INTERRUPTS
    return yLimitSwitch;
#else
    int sum = digitalRead(Y_LIMIT_SWITCH_PIN);
    if (!sum) return sum;

    for (int i = 0; i < 4; i++) {
        sum += digitalRead(Y_LIMIT_SWITCH_PIN);
    }
    return sum == 5;
#endif
}

boolean GantryConfiguration::zLimitSwitchTriggered() {
#ifdef LIMIT_SWITCH_INTERRUPTS
    return zLimitSwitch;
#else
    int sum = digitalRead(Z_LIMIT_SWITCH_PIN);
    if (!sum) return sum;

    for (int i = 0; i < 4; i++) {
        sum += digitalRead(Z_LIMIT_SWITCH_PIN);
    }
    return sum == 5;
#endif
}

boolean GantryConfiguration::thetaLimitSwitchTriggered() {
#ifdef LIMIT_SWITCH_INTERRUPTS
    return thetaLimitSwitch;
#else
    return digitalRead(THETA_LIMIT_SWITCH_PIN);
#endif

}

static void clickX1() {
    x1LimitSwitch = digitalRead(X1_LIMIT_SWITCH_PIN);
}

static void clickX2() {
    x2LimitSwitch = digitalRead(X2_LIMIT_SWITCH_PIN);
}

static void clickY() {
    yLimitSwitch = digitalRead(Y_LIMIT_SWITCH_PIN);
}

static void clickZ() {
    zLimitSwitch = digitalRead(Z_LIMIT_SWITCH_PIN);
}

static void clickTheta() {
    thetaLimitSwitch = digitalRead(THETA_LIMIT_SWITCH_PIN);
}


boolean GantryConfiguration::masterLimitSwitchTriggered() {
    return digitalRead(MASTER_LIMIT_SWITCH);
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

void GantryConfiguration::execute(Command &command) {
    command.execute(*this);
}

void GantryConfiguration::execute(String &command) {
    CommandParser::parse(command).execute(*this);
}

void GantryConfiguration::execute(char *command) {
    CommandParser::parse(command).execute(*this);
}

void GantryConfiguration::execute(Command *commands, int numCommands) {
    for (int i = 0; i < numCommands; i++) {
        commands[i].execute(*this);
    }
}

void GantryConfiguration::execute(String *commands, int numCommands) {
    for (int i = 0; i < numCommands; i++) {
        CommandParser::parse(commands[i]).execute(*this);
    }
}

void GantryConfiguration::execute(char **commands, int numCommands) {
    for (int i = 0; i < numCommands; i++) {
        CommandParser::parse(commands[i]).execute(*this);
    }
}

