#pragma once
#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <math.h>

namespace ServoConstants
{
    constexpr int minServoPWM { 150 };
    constexpr int maxServoPWM { 600 };
    constexpr float defaultStepDelay { 25 };
    constexpr float defaultStepSize { 1 };
    constexpr float minRotation { 0.01 };
    
    inline constexpr float degToPWM(float degrees){  return degrees * 2.5 + 150;  }
}

class Joint
{
private:
    float m_rotation{ 90 };
    float m_targetRotation{ 90 };
    float m_stepSize{ ServoConstants::defaultStepSize };
    float m_stepDelay{ ServoConstants::defaultStepDelay };
    unsigned long lastMoveTime{};
    int m_pwmIndex {};

    

public:
    Joint(int pwmIndex, float rotation=0)
        : m_rotation { rotation },
        m_pwmIndex { pwmIndex}
    {
        
    }
    void setRotation(Adafruit_PWMServoDriver& servoDriver, float degrees)
    {        
        if(degrees < 0) degrees = 0; // make sure value is in range
        else if(degrees > 180) degrees = 180;

        m_rotation = degrees;
        Serial.println(degrees);
        servoDriver.setPWM(m_pwmIndex, 0, ServoConstants::degToPWM(degrees));
    }   

    void setRotationSmooth(Adafruit_PWMServoDriver& servoDriver, float degrees,
        float stepSize=ServoConstants::defaultStepSize, float stepDelay=ServoConstants::defaultStepDelay)
    {
        if(degrees < 0) degrees = 0;
        else if(degrees > 180) degrees = 180;

        m_targetRotation = degrees;
        m_stepDelay = stepDelay;
        m_stepSize = stepSize;

        while(abs(m_rotation - m_targetRotation) > ServoConstants::minRotation)
        {
            if(millis() - lastMoveTime >= m_stepDelay)
            {
                float nextRotation = m_rotation;

                if(m_rotation < m_targetRotation)
                {
                    nextRotation += m_stepSize;
                    if(nextRotation > m_targetRotation) nextRotation = m_targetRotation;
                }
                else
                {
                    nextRotation -= m_stepSize;
                    if(nextRotation < m_targetRotation) nextRotation = m_targetRotation;
                }
                
                setRotation(servoDriver, nextRotation);
                lastMoveTime = millis();
            }
        }
    }
};