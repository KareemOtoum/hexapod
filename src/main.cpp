#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include "leg.h"

static Adafruit_PWMServoDriver servoDriver {};
Leg leg { };

void setup() 
{
    Serial.begin(9600);
    Serial.println("Starting hexapod...");

    servoDriver.begin();
    servoDriver.setPWMFreq(60);
    leg.resetRotations(servoDriver);
}

void loop() 
{   
}
