#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

class MotorDriver {
public:
    // Initialize motor driver pins
    static void initializeMotorDriver();

    // Control left motor
    static void setLeftMotorPower(int value);

    // Control right motor  
    static void setRightMotorPower(int value);

    // Control both motors
    static void setMotorsPower(int left, int right);
};

#endif // MOTORDRIVER_H