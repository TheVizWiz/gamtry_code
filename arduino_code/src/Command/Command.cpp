//
// Created by thevizwiz on 2/16/2024.
//

#include "Command.h"
#include "CommandParser.h"
#include "Logger/Logger.h"
#include "Vector.h"


const Command Command::NO_COMMAND = Command(CommandType::NONE);

static Logger logger = Logger("Command");

Command::Command() {

}

Command::Command(CommandType type) : Command() {
    this->type = type;
}

void Command::execute(GantryConfiguration &gantry) {

    logger.logBare("");
    char *toString = this->toString();
    logger.log("Running new command. Command: %s", toString);
    free(toString);

    switch (this->type) {
        case NONE:
            break;
        case BASE:
            executeBase(gantry);
            break;
        case HEAD_CHANGE:
            executeHeadChange(gantry);
            break;
        case SPECIAL_COMMAND:
            executeSpecial(gantry);
            break;
        case MACRO:
            break;
        case GRIPPER_COMMAND:
            executeGripper(gantry);
            break;
        case WRITE_COMMAND:
            executeWrite(gantry);
            break;
        case GLUE_COMMAND:
            executeGlue(gantry);
            break;
        case READ_COMMAND:
            executeRead(gantry);
            break;
    }
}

void Command::executeBase(GantryConfiguration &gantry) {




//    Serial.print("Gantry x position: ");
//    Serial._println(gantry.position.x);
//    Serial.print("Wanted x position: ");
//    Serial._println(x);
    logger.log("Executing base command.");


    if (z_changed) {
        logger.log("Z moved. Calculating new z...");
        z = max(min(z, MAX_Z_MM), 0);
        double dz = z_changed ? (z - gantry.position.z) : 0; // mm
        double min_z_time = abs(dz) / Z_MAX_MM_PER_SECOND; // mm / (mm / s) = mm * s / mm = s = seconds
        time = max(min_z_time, time);
        double z_speed_mm = dz / time; // mm per second
        double z_speed_steps = z_speed_mm * Z_STEPS_PER_MM; // steps per second
        gantry.z_motor.moveTo(z * Z_STEPS_PER_MM);
        gantry.z_motor.setSpeed(z_speed_steps);
        logger.log("moving Z axis at % steps/s until %fmm.", z_speed_steps, z);
        while (gantry.z_motor.currentPosition() != gantry.z_motor.targetPosition())
            gantry.z_motor.runSpeedToPosition();
        gantry.updatePosition();
    }

    x = max(min(x, MAX_X_MM), 0);
    y = max(min(y, MAX_Y_MM), 0);
    theta = max(min(theta, MAX_THETA_DEG), 0);


    double dx = x_changed ? (x - gantry.position.x) : 0; // mm
    double dy = y_changed ? (y - gantry.position.y) : 0; // mm
    double dtheta = theta_changed ? (theta - gantry.position.theta) : 0; // mm



    double min_x_time = abs(dx) / X_MAX_MM_PER_SECOND; // mm / (mm / s) = mm * s / mm = s = seconds
    double min_y_time = abs(dy) / Y_MAX_MM_PER_SECOND; // mm / (mm / s) = mm * s / mm = s = seconds
    double min_theta_time = abs(dtheta) / THETA_MAX_DEG_PER_SECOND; // deg / (deg / s) = deg * s / deg = s = seconds
//

    time = max(min_x_time, time);
    time = max(min_y_time, time);
    time = max(min_theta_time, time);



    logger.log("time for total movement:%f.", time);


//    Serial.print("calculated time: ");
//    Serial._println(time);


    double x_speed_mm = dx / time; // mm per second
    double y_speed_mm = dy / time; // mm per second
    double theta_speed_mm = dtheta / time; // deg per second


    double x_speed_steps = x_speed_mm * X_STEPS_PER_MM; // steps per second
    double y_speed_steps = y_speed_mm * Y_STEPS_PER_MM; // steps per second
    double theta_speed_steps = theta_speed_mm * THETA_STEPS_PER_DEG; // steps per second

//    Serial.print("Calculated speed: ");
//    Serial._println(x_speed_steps);

    if (x_changed) {
        gantry.x1_motor.moveTo(x * X_STEPS_PER_MM);
        gantry.x2_motor.moveTo(x * X_STEPS_PER_MM);
        gantry.x1_motor.setSpeed(x_speed_steps);
        gantry.x2_motor.setSpeed(x_speed_steps);
    }


    if (y_changed) {
        gantry.y_motor.moveTo(y * Y_STEPS_PER_MM);
        gantry.y_motor.setSpeed(y_speed_steps);
    }

    if (theta_changed) {
        gantry.theta_motor.moveTo(theta * THETA_STEPS_PER_DEG);
        gantry.theta_motor.setSpeed(theta_speed_steps);
    }


    long x_wanted = x * X_STEPS_PER_MM;
    long y_wanted = y * Y_STEPS_PER_MM;
    long z_wanted = z * Z_STEPS_PER_MM;
    long theta_wanted = theta * THETA_STEPS_PER_DEG;


    while (true) {


        boolean x_finished = !x_changed ||
                             (gantry.x1_motor.currentPosition() == gantry.x1_motor.targetPosition() &&
                              gantry.x2_motor.currentPosition() == gantry.x2_motor.targetPosition());
        boolean y_finished = !y_changed ||
                             gantry.y_motor.currentPosition() == gantry.y_motor.targetPosition();
//        boolean z_finished = !z_changed ||
//                             gantry.z_motor.currentPosition() == z_wanted;
        boolean theta_finished = !theta_changed ||
                                 gantry.theta_motor.currentPosition() == gantry.theta_motor.targetPosition();


        if (x_finished && y_finished && theta_finished)
            break;


        if (x_changed)
            gantry.x1_motor.runSpeedToPosition();
        if (x_changed)
            gantry.x2_motor.runSpeedToPosition();
        if (y_changed)
            gantry.y_motor.runSpeedToPosition();

        if (theta_changed)
            gantry.theta_motor.runSpeedToPosition();


        gantry.updatePosition();
    }

    gantry.x1_motor.setSpeed(0);
    gantry.x2_motor.setSpeed(0);
    gantry.y_motor.setSpeed(0);
    gantry.z_motor.setSpeed(0);
    gantry.theta_motor.setSpeed(0);

    logger.log("Successfully executed base command.");
    char *currentGantryPos = gantry.position.toString();
    logger.log("Current gantry position: %s.", currentGantryPos);
    free(currentGantryPos);

}

void Command::executeHeadChange(GantryConfiguration &gantry) {

    logger.log("Received Head Change command. Executing...");
    logger.log("Current gantry position: %d. New wanted: %d", gantry.position.head, head);

    switch (gantry.position.head) {
        case 0:
            switch (head) {
                case 0:
                    break;
                case 1:
                    logger.log("Switching from no head to Gripper.");
                    String commands[] = {
                            "Z95",
                            "Y266",
                            "X341",
                            "Z65",
                            "Z67",
                            "X280 t2",
                            String("G") + MIN_G
                    };
                    gantry.execute(commands, 7);
                    logger.log("successfully changed head to head 1: Gripper.");
                    break;
            }
            gantry.position.head = 1;
            break;
        case 1:
            switch (head) {
                case 0:
                    logger.log("Switching from Gripper head to no head.");
                    String commands[] = {
                            "Z67",
                            "Y266",
                            "X280",
                            "X341 t2",
                            "Z48",
                            "X280"
                    };
                    gantry.execute(commands, 6);
                    logger.log("Successfully switched from Gripper to no head.");
                    break;
            }
            gantry.position.head = 0;
            break;
        case 2:
            break;
    }

}

void Command::executeSpecial(GantryConfiguration &gantry) {

    switch (this->code) {
        case CommandParser::COMMAND_SPECIAL_HOME:
            executeHoming(gantry);
            break;
        case CommandParser::COMMAND_SPECIAL_HOME_X:
            gantry.homeXAxis();
            break;
        case CommandParser::COMMAND_SPECIAL_HOME_Y:
            gantry.homeYAxis();
            break;
        case CommandParser::COMMAND_SPECIAL_HOME_Z:
            gantry.homeZAxis();
            break;
        case CommandParser::COMMAND_SPECIAL_HOME_THETA:
            gantry.homeThetaAxis();
            break;
        case CommandParser::COMMAND_SPECIAL_HOME_THETA + 1:

            CommandParser::parse("Z10").execute(gantry);
            CommandParser::parse("t2 X270 Y410").execute(gantry);
            CommandParser::parse("Z0").execute(gantry);
            CommandParser::parse("t2 X220 Y400").execute(gantry);
            CommandParser::parse("t2 X245 Y390").execute(gantry);
            CommandParser::parse("t2 X220 Y380").execute(gantry);
            CommandParser::parse("t2 X270 Y370").execute(gantry);
            CommandParser::parse("Z10").execute(gantry);

            CommandParser::parse("t2 X270 Y290").execute(gantry);
            CommandParser::parse("Z0").execute(gantry);
            CommandParser::parse("t2 X270 Y330").execute(gantry);
            CommandParser::parse("t2 X220 Y330").execute(gantry);
            CommandParser::parse("t2 X220 Y290").execute(gantry);
            CommandParser::parse("Z10").execute(gantry);
            CommandParser::parse("t2 X245 Y330").execute(gantry);
            CommandParser::parse("Z0").execute(gantry);
            CommandParser::parse("t2 X245 Y310").execute(gantry);
            CommandParser::parse("Z10").execute(gantry);

            CommandParser::parse("t2 X200 Y350").execute(gantry);
            CommandParser::parse("Z0").execute(gantry);
            CommandParser::parse("t2 X208 Y342").execute(gantry);
            CommandParser::parse("t2 X208 Y333").execute(gantry);
            CommandParser::parse("t2 X200 Y325").execute(gantry);
            CommandParser::parse("t2 X170 Y350").execute(gantry);
            CommandParser::parse("t2 X200 Y375").execute(gantry);
            CommandParser::parse("t2 X208 Y367").execute(gantry);
            CommandParser::parse("t2 X208 Y358").execute(gantry);
            CommandParser::parse("t2 X200 Y350").execute(gantry);
            CommandParser::parse("Z10").execute(gantry);

            CommandParser::parse("t2 X160 Y370").execute(gantry);
            CommandParser::parse("Z0").execute(gantry);
            CommandParser::parse("t2 X160 Y410").execute(gantry);
            CommandParser::parse("t2 X110 Y410").execute(gantry);
            CommandParser::parse("t2 X110 Y370").execute(gantry);
            CommandParser::parse("t2 X135 Y370").execute(gantry);
            CommandParser::parse("t2 X135 Y390").execute(gantry);
            CommandParser::parse("t2 X125 Y390").execute(gantry);
            CommandParser::parse("Z10").execute(gantry);

            CommandParser::parse("t2 X160 Y330").execute(gantry);
            CommandParser::parse("Z0").execute(gantry);
            CommandParser::parse("t2 X110 Y330").execute(gantry);
            CommandParser::parse("t2 X110 Y290").execute(gantry);
            CommandParser::parse("t2 X160 Y290").execute(gantry);
            CommandParser::parse("Z10").execute(gantry);
            CommandParser::parse("t2 X150 Y150").execute(gantry);


            break;

    }

}

void Command::executeHoming(GantryConfiguration &gantry) {
    gantry.homeXAxis();
    gantry.homeYAxis();
    gantry.homeZAxis();
    gantry.homeThetaAxis();
    CommandParser::parse("X250 Y250 Z20 T90").execute(gantry);
//    gantry.homeThetaAxis();
}

void Command::executeGripper(GantryConfiguration &gantry) {
    logger.log("Executing gripper command.");

    g = max(min(g, MAX_G), MIN_G);
    gantry.gripperServo.write(g);
    delay(15);
}

void Command::executeRead(GantryConfiguration &gantry) {

    File file = SD.open(String("commands/") + fileName,FILE_READ);
    if (!file) {
        logger.log("no such file exists. Unable to read command");
    }


    logger.log("File with filename %s found. Running all commands...", file.name());

    String allLines = file.readString();

    char currentLine[25];

    int currentLocation = 0;
    int nextLineLocation = 0;

    while (nextLineLocation < allLines.length()) {
        if (allLines[nextLineLocation] == '\n') {
            memcpy(currentLine, allLines.c_str() + currentLocation, nextLineLocation - currentLocation);
            currentLocation = nextLineLocation + 1;
            nextLineLocation = currentLocation;
            logger.log(F("Line read from file: %s"), currentLine);
            CommandParser::parse(currentLine).execute(gantry);
        }
        nextLineLocation++;
    }

    String line = allLines.substring(currentLocation, nextLineLocation);
    logger.log(F("Line read from file: %s"), currentLine);
    CommandParser::parse(line).execute(gantry);

}


typedef struct LetterData {
    float bottomLeftX;
    float bottomLeftY;
    float width;
    float height;
} LetterData;

static String getXYWriteCoordinates(LetterData &data, float x, float y) {

    String command = "t2 ";
    x = x * data.width + data.bottomLeftX;
    y = y * data.height + data.bottomLeftY;
    command = command + "X" + x + " Y" + y;
    return command;
}


void Command::drawLetter(GantryConfiguration &gantry,
                         char letter,
                         float x_start,
                         float y_start,
                         float width,
                         float height,
                         float z_start,
                         float z_jump) {


    LetterData data = {
            x_start,
            y_start,
            width,
            height
    };

    String z_string = String("Z");
    String down = z_string + z_start;
    String up = z_string + z_jump;

    gantry.execute(up);


    logger.log(F("Writing char %s with (X, Y, Z, W, H) = (%f, %f, %f, %f, %f)"), letter, x_start, y_start, z_start, width, height);

    if (letter == '0') {
        String commands[] = {
                getXYWriteCoordinates(data, 1 / 4.0, 0),
                down,
                getXYWriteCoordinates(data, 0, 1 / 6.0),
                getXYWriteCoordinates(data, 0, 5 / 6.0),
                getXYWriteCoordinates(data, 1 / 4.0, 1),
                getXYWriteCoordinates(data, 3 / 4.0, 1),
                getXYWriteCoordinates(data, 1, 5 / 6.0),
                getXYWriteCoordinates(data, 1, 1 / 6.0),
                getXYWriteCoordinates(data, 3 / 4.0, 0),
                getXYWriteCoordinates(data, 1 / 4.0, 0),
                up,
                getXYWriteCoordinates(data, 1 / 8.0, 1 / 12.0),
                down,
                getXYWriteCoordinates(data, 7 / 8.0, 11 / 12.0),};
        gantry.execute(commands, 14);
    } else if (letter == '1') {
        String commands[] = {
                getXYWriteCoordinates(data, 0, 0),
                down,
                getXYWriteCoordinates(data, 1, 0),
                up,
                getXYWriteCoordinates(data, 0.5, 0),
                down,
                getXYWriteCoordinates(data, 0.5, 1),
                getXYWriteCoordinates(data, 0.125, 0.75),
        };
        gantry.execute(commands, 8);

    } else if (letter == '2') {
        String commands[] = {
                getXYWriteCoordinates(data, 0, 0.75),
                down,
                getXYWriteCoordinates(data, 1 / 3.0, 1),
                getXYWriteCoordinates(data, 2 / 3.0, 1),
                getXYWriteCoordinates(data, 1, 3 / 4.0),
                getXYWriteCoordinates(data, 1, 1 / 2.0),
                getXYWriteCoordinates(data, 0, 0),
                getXYWriteCoordinates(data, 1, 0),
        };
        gantry.execute(commands, 8);

    } else if (letter == '3') {

        String commands[] = {
                getXYWriteCoordinates(data, 0, 0.75),
                down,
                getXYWriteCoordinates(data, 1 / 3.0, 1),
                getXYWriteCoordinates(data, 2 / 3.0, 1),
                getXYWriteCoordinates(data, 1, 3 / 4.0),
                getXYWriteCoordinates(data, 1, 7 / 12.0),
                getXYWriteCoordinates(data, 1 / 2.0, 1 / 2.0),
                getXYWriteCoordinates(data, 1, 5 / 12.0),
                getXYWriteCoordinates(data, 1, 1 / 4.0),
                getXYWriteCoordinates(data, 2 / 3.0, 0),
                getXYWriteCoordinates(data, 1 / 3.0, 0),
                getXYWriteCoordinates(data, 0, 1 / 4.0)
        };
        gantry.execute(commands, 12);
    } else if (letter == '4') {

        String commands[] = {
                getXYWriteCoordinates(data, 0, 1),
                down,
                getXYWriteCoordinates(data, 0, 1 / 2.0),
                getXYWriteCoordinates(data, 1, 1 / 2.0),
                up,
                getXYWriteCoordinates(data, 1, 1),
                down,
                getXYWriteCoordinates(data, 1, 0)
        };
        gantry.execute(commands, 8);

    } else if (letter == '5') {
        String commands[] = {
                getXYWriteCoordinates(data, 1, 1),
                down,
                getXYWriteCoordinates(data, 0, 1),
                getXYWriteCoordinates(data, 0, 0.5),
                getXYWriteCoordinates(data, 1 / 3.0, 3 / 5.0),
                getXYWriteCoordinates(data, 2 / 3.0, 3 / 5.0),
                getXYWriteCoordinates(data, 1, 2 / 5.0),
                getXYWriteCoordinates(data, 1, 1 / 5.0),
                getXYWriteCoordinates(data, 2 / 3.0, 0),
                getXYWriteCoordinates(data, 1 / 3.0, 0),
                getXYWriteCoordinates(data, 0, 1 / 5.0)
        };
        gantry.execute(commands, 11);

    } else if (letter == '6') {
        String commands[] = {
                getXYWriteCoordinates(data, 1, 3 / 4.0),
                down,
                getXYWriteCoordinates(data, 1, 4 / 5.0),
                getXYWriteCoordinates(data, 2 / 3.0, 1),
                getXYWriteCoordinates(data, 1 / 3.0, 1),
                getXYWriteCoordinates(data, 0, 3 / 4.0),
                getXYWriteCoordinates(data, 0, 1 / 5.0),
                getXYWriteCoordinates(data, 1 / 3.0, 0),
                getXYWriteCoordinates(data, 2 / 3.0, 0),
                getXYWriteCoordinates(data, 1, 1 / 5.0),
                getXYWriteCoordinates(data, 1, 2 / 5.0),
                getXYWriteCoordinates(data, 2 / 3.0, 3 / 5.0),
                getXYWriteCoordinates(data, 1 / 3.0, 3 / 5.0),
                getXYWriteCoordinates(data, 0, 2 / 5.0),
        };
        gantry.execute(commands, 14);

    } else if (letter == '7') {

        String commands[] = {
                getXYWriteCoordinates(data, 0, 1),
                down,
                getXYWriteCoordinates(data, 1, 1),
                getXYWriteCoordinates(data, 0, 0),
                up,
                getXYWriteCoordinates(data, 1 / 4.0, 1 / 2.0),
                down,
                getXYWriteCoordinates(data, 3 / 4.0, 1 / 2.0)
        };
        gantry.execute(commands, 8);

    } else if (letter == '8') {

        String commands[] = {
                getXYWriteCoordinates(data, 3 / 4.0, 1 / 2.0),
                down,
                getXYWriteCoordinates(data, 1, 4 / 6.0),
                getXYWriteCoordinates(data, 1, 5 / 6.0),
                getXYWriteCoordinates(data, 3 / 4.0, 1),
                getXYWriteCoordinates(data, 1 / 4.0, 1),
                getXYWriteCoordinates(data, 0, 5 / 6.0),
                getXYWriteCoordinates(data, 0, 4 / 6.0),
                getXYWriteCoordinates(data, 1 / 4.0, 1 / 2.0),
                getXYWriteCoordinates(data, 3 / 4.0, 1 / 2.0),
                getXYWriteCoordinates(data, 1, 2 / 6.0),
                getXYWriteCoordinates(data, 1, 1 / 6.0),
                getXYWriteCoordinates(data, 3 / 4.0, 0),
                getXYWriteCoordinates(data, 1 / 4.0, 0),
                getXYWriteCoordinates(data, 0, 1 / 6.0),
                getXYWriteCoordinates(data, 0, 2 / 6.0),
                getXYWriteCoordinates(data, 1 / 4.0, 1 / 2.0),
        };
        gantry.execute(commands, 17);
    } else if (letter == '9') {

        String commands[] = {
                getXYWriteCoordinates(data, 1, 4 / 6.0),
                down,
                getXYWriteCoordinates(data, 3 / 4.0, 1 / 2.0),
                getXYWriteCoordinates(data, 1 / 4.0, 1 / 2.0),
                getXYWriteCoordinates(data, 0, 4 / 6.0),
                getXYWriteCoordinates(data, 0, 5 / 6.0),
                getXYWriteCoordinates(data, 1 / 4.0, 1),
                getXYWriteCoordinates(data, 3 / 4.0, 1),
                getXYWriteCoordinates(data, 1, 5 / 6.0),
                getXYWriteCoordinates(data, 1, 4 / 6.0),
                getXYWriteCoordinates(data, 1, 0)
        };
        gantry.execute(commands, 11);
    } else if (letter == 'M') {
        String commands[] = {
                getXYWriteCoordinates(data, 0, 0),
                down,
                getXYWriteCoordinates(data, 0, 1),
                getXYWriteCoordinates(data, 1 / 2.0, 1 / 2.0),
                getXYWriteCoordinates(data, 1, 1),
                getXYWriteCoordinates(data, 1, 0)
        };
        gantry.execute(commands, 6);

    } else if (letter == 'E') {
        String commands[] = {
                getXYWriteCoordinates(data, 1, 1),
                down,
                getXYWriteCoordinates(data, 0, 1),
                getXYWriteCoordinates(data, 0, 0),
                getXYWriteCoordinates(data, 1, 0),
                up,
                getXYWriteCoordinates(data, 0, 1 / 2.0),
                down,
                getXYWriteCoordinates(data, 2 / 3.0, 1 / 2.0)
        };
        gantry.execute(commands, 9);


    } else if (letter == 'G') {
        String commands[] = {
                getXYWriteCoordinates(data, 1, 1),
                down,
                getXYWriteCoordinates(data, 0, 1),
                getXYWriteCoordinates(data, 0, 0),
                getXYWriteCoordinates(data, 1, 0),
                getXYWriteCoordinates(data, 1, 1 / 2.0),
                getXYWriteCoordinates(data, 1 / 3.0, 1 / 2.0),
                getXYWriteCoordinates(data, 1 / 3.0, 1 / 3.0)
        };
        gantry.execute(commands, 8);
    }

    gantry.execute(up);
}

void Command::executeWrite(GantryConfiguration &gantry) {

    float z_start = this->z_changed ? this->z : gantry.position.z;
    float z_jump = z_start + 10.0; //10mm jump
    float y_start = this->y_changed ? this->y : gantry.position.y;
    float width = base_size;
    float height = width * 1.5f; // aspect ratio
    float x_beginning = this->x_changed ? this->x : gantry.position.x;

    for (int i = 0; i < letters.length(); i++) {
        char letter = letters[i];
        float x_start = x_beginning + char_x_multiplier * width * i;
        drawLetter(gantry, letter, x_start, y_start, width, height, z_start, z_jump);
    }
}

void Command::executeGlue(GantryConfiguration &gantry) {
    logger.log("executing glue command.");
    time = time_changed ? time : 1.0;
    glue_speed = max(-1, min(1, glue_speed));
    int speed = 255 * abs(glue_speed);
    logger.log(F("Running glue at %f for %f seconds"), speed, time);


    float startTime = millis();
    if (speed < 0 ) {
        analogWrite(GLUE_PIN_2, LOW);
        analogWrite(GLUE_PIN_1, speed);
    } else {
        analogWrite(GLUE_PIN_1, LOW);
        analogWrite(GLUE_PIN_2, speed);
    }
    delay(time * 1000);

    logger.log("Finished running glue gun. setting glue speed to 0...");

    analogWrite(GLUE_PIN_2, LOW);
    analogWrite(GLUE_PIN_1, LOW);

}

boolean Command::isNoCommand() const {
    return this->type == CommandType::NONE;
}

char * Command::toString() {

    char *message = static_cast<char *>(malloc(50 * sizeof(char)));

    String s;

    switch (type) {

        case NONE:
            return "{NO_COMMAND}";
            break;
        case BASE:
            s = "{";

            if (x_changed) s = s + "X: " + x + " ";
            if (y_changed) s = s + "Y: " + y + " ";
            if (z_changed) s = s + "Z: " + z + " ";
            if (time_changed) s = s + "time: " + time + " ";
            if (theta_changed) s = s + "theta: " + theta + " ";
            if (head_1_changed) s = s + "head_1: " + head_1 + " ";
            s = s.substring(0, s.length() - 1) + "}";

            sprintf(message, "%s", s.c_str());

            break;
        case HEAD_CHANGE:
            return "{HEAD_CHANGE: %d}";
            break;
        case SPECIAL_COMMAND:
            sprintf(message, "{SPECIAL: %d}" + code);
            return message;
            break;
        case MACRO:
            break;
        case WRITE_COMMAND:
            sprintf(message, "%s", (String("{WRITE: ") + letters + "width: " + base_size + " x_dist: " + char_x_multiplier + "}").c_str());
            return message;
        default:
            s = "COMMAND";
    }

    return message;

}


