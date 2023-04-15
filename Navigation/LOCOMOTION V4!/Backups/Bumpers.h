BumperSwitches Bumpers;
//
void BumperProcess(void)
{

    if (debug)
    {
        Serial.println("Bumper Process Started");
        Serial.print("X: ");
        Serial.print(BotOrientation.X);
        Serial.print(" Y: ");
        Serial.print(BotOrientation.Y);
        Serial.print(" Theta: ");
        Serial.println(BotOrientation.Theta);
        Serial.println();
        Serial.print("Switches State: ");
        Serial.println(SwitchesState);
        Serial.println();
    };
    // NavigationProcessRunning = false;
    Controller.emergencyStop();
    // Nudging = true;
    //  delay(100);
    if ((BotOrientation.Theta <= PI / 3) && (BotOrientation.Theta >= -(PI / 3)))
    { // We are pointed North
        //
        if (debug)
            Serial.println("Facing North");

        switch (SwitchesState)
        {
        case FRONT_PRESSED:
            BotOrientation.Y = NORTH_WALL;
            Serial.println("Facing North, Front Pressed");
            Nudge(NORTH);
            break;
        case LEFT_PRESSED:
            BotOrientation.X = WEST_WALL;
            Serial.println("Facing North, Left Pressed");
            Nudge(WEST);
            break;
        case RIGHT_PRESSED:
            BotOrientation.X = EAST_WALL;
            Serial.println("Facing North, Right Pressed");
            Nudge(EAST);
            break;
        case BACK_PRESSED:
            BotOrientation.Y = SOUTH_WALL;
            Serial.println("Facing North, Back Pressed");
            Nudge(SOUTH);
            break;
        };
        SwitchesState = NONE_PRESSED;

        return;
    };
    if ((BotOrientation.Theta >= PI / 6) && (BotOrientation.Theta <= (11 / 16) * PI))
    { // We are pointed East
        //
        switch (SwitchesState)
        {
        case FRONT_PRESSED:
            BotOrientation.X = EAST_WALL;
            Nudge(EAST);
            break;
        case LEFT_PRESSED:
            BotOrientation.Y = NORTH_WALL;
            Nudge(NORTH);
            break;
        case RIGHT_PRESSED:
            BotOrientation.Y = SOUTH_WALL;
            Nudge(SOUTH);
            break;
        case BACK_PRESSED:
            BotOrientation.X = WEST_WALL;
            Nudge(WEST);
            break;
        };
        SwitchesState = NONE_PRESSED;
        return;
    };
    if ((BotOrientation.Theta <= -PI / 6) && (BotOrientation.Theta >= -(11 / 16) * PI))
    { // We are pointed West
        switch (SwitchesState)
        {
        case FRONT_PRESSED:
            BotOrientation.X = WEST_WALL;
            Nudge(WEST);
            break;
        case LEFT_PRESSED:
            BotOrientation.Y = SOUTH_WALL;
            Nudge(SOUTH);
            break;
        case RIGHT_PRESSED:
            BotOrientation.Y = NORTH_WALL;
            Nudge(NORTH);
            break;
        case BACK_PRESSED:
            BotOrientation.X = EAST_WALL;
            Nudge(EAST);
            break;
        };
        //
        SwitchesState = NONE_PRESSED;
        return;
    };
    if ((BotOrientation.Theta >= (5 / 6) * PI) || (BotOrientation.Theta <= -(5 / 6) * PI))
    { // We are pointed South
        switch (SwitchesState)
        {
        case FRONT_PRESSED:
            BotOrientation.Y = SOUTH_WALL;
            Nudge(SOUTH);
            break;
        case LEFT_PRESSED:
            BotOrientation.X = EAST_WALL;
            Nudge(EAST);
            break;
        case RIGHT_PRESSED:
            BotOrientation.X = WEST_WALL;
            Nudge(WEST);
            break;
        case BACK_PRESSED:
            BotOrientation.Y = NORTH_WALL;
            Nudge(NORTH);
            break;
        };
        SwitchesState = NONE_PRESSED;
        return;

        //
    };
    // SwitchesState = NONE_PRESSED;
    // MoveState = IDLE;
};
void Nudge(char NorthSouthOrWest)
{
    Serial.println("BEFORE NUDGE");
    Serial.print("X: ");
    Serial.print(BotOrientation.X);
    Serial.print(" Y: ");
    Serial.print(BotOrientation.Y);
    Serial.print(" Theta: ");
    Serial.println(BotOrientation.Theta);
    Serial.println(NorthSouthOrWest);
    switch (NorthSouthOrWest)
    {
    case NORTH:

        motor_1.setTargetRel(-NUDGE_DISTANCE);
        motor_2.setTargetRel(NUDGE_DISTANCE);
        motor_3.setTargetRel(-NUDGE_DISTANCE);
        motor_4.setTargetRel(NUDGE_DISTANCE);
        break;
    case SOUTH:
        motor_1.setTargetRel(NUDGE_DISTANCE);
        motor_2.setTargetRel(-NUDGE_DISTANCE);
        motor_3.setTargetRel(NUDGE_DISTANCE);
        motor_4.setTargetRel(-NUDGE_DISTANCE);

        break;
    case WEST:
        motor_1.setTargetRel(NUDGE_DISTANCE);
        motor_2.setTargetRel(NUDGE_DISTANCE);
        motor_3.setTargetRel(-NUDGE_DISTANCE);
        motor_4.setTargetRel(-NUDGE_DISTANCE);
        // UpdateDesiredPose(BotOrientation.Theta, BotOrientation.X + 5, BotOrientation.Y);
        // ComputeTranslation();
        break;
    case EAST:
        motor_1.setTargetRel(-NUDGE_DISTANCE);
        motor_2.setTargetRel(-NUDGE_DISTANCE);
        motor_3.setTargetRel(NUDGE_DISTANCE);
        motor_4.setTargetRel(NUDGE_DISTANCE);
        // UpdateDesiredPose(BotOrientation.Theta, BotOrientation.X - 5, BotOrientation.Y);
        // ComputeTranslation();
        break;
    };
    motor_1
        .setMaxSpeed(NUDGE_SPEED)
        .setAcceleration(NUDGE_ACCEL);
    motor_2
        .setMaxSpeed(NUDGE_SPEED)
        .setAcceleration(NUDGE_ACCEL);
    motor_3
        .setMaxSpeed(NUDGE_SPEED)
        .setAcceleration(NUDGE_ACCEL);
    motor_4
        .setMaxSpeed(NUDGE_SPEED)
        .setAcceleration(NUDGE_ACCEL);
    Controller.move(motor_1, motor_2, motor_3, motor_4);
    delay(5000);
    Serial.println("after NUDGE");
    Serial.print("X: ");
    Serial.print(BotOrientation.X);
    Serial.print(" Y: ");
    Serial.print(BotOrientation.Y);
    Serial.print(" Theta: ");
    Serial.println(BotOrientation.Theta);
    SwitchesState = NONE_PRESSED;
};

void SweepTheBoard(void)
{
    // UpdateDesiredPose(BotOrientation.Theta, SWEEP_START_X, SWEEP_START_Y);
    // ComputeTranslation(0);
    Serial.print("\n\n\n");
    Serial.print(BotOrientation.Theta);
    Serial.print(BotOrientation.Y);
    Serial.print(BotOrientation.X);
    Serial.print("\n\n\n");
    int SweepY = BotOrientation.Y;
    for (int i = 0; i < SWEEP_COUNT; i++)
    {
        UpdateDesiredPose(BotOrientation.Theta, -SWEEP_MAX_X, SweepY);
        ComputeTranslation(0);
        SweepY += SWEEP_DISTANCE_INCREMENT;
        UpdateDesiredPose(BotOrientation.Theta, WEST_WALL, SweepY);
        ComputeTranslation(0);
        SweepY += SWEEP_DISTANCE_INCREMENT;
    };
};