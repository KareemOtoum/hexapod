#pragma once
#include "joint.h"

class Leg
{
private:
    const float m_l1{ 8.0 };
    const float m_l2{ 21.0 };
    const float m_l3{ 25.0 };
    const float m_shoulderZOffset{ 4.0 };

    Joint m_base{ 0 };
    Joint m_shoulder{ 1 };
    Joint m_elbow{ 2 };

public:
    Leg() 
    {
        
    }

    void moveToPosition(Adafruit_PWMServoDriver servoDriver, float x, float y, float z)
    {
        // base rotation
        float theta1 = atan2(z - m_shoulderZOffset, x) * (180.0 / M_PI);

        // target in shoulder's local frame
        float zPrime = z - m_shoulderZOffset;
        float yPrime = y - m_l1;

        // distance from shoulder to target
        float D = sqrt(zPrime * zPrime + yPrime * yPrime);

        // Check if target is reachable
        if (D > (m_l2 + m_l3) || D < fabs(m_l2 - m_l3))
        {
            Serial.println("Error: target position is out of reach.");
            return;
        }

        // Compute elbow angle theta3
        float cosTheta3 = (m_l2 * m_l2 + m_l3 * m_l3 - D * D) / (2 * m_l2 * m_l3);
        if (cosTheta3 < -1.0 || cosTheta3 > 1.0)
        {
            Serial.println("Error: invalid position cannot compute elbow angle.");
            return;
        }
        float theta3 = acos(cosTheta3) * (180.0 / M_PI);

        // Compute shoulder angle theta2
        float phi = atan2(zPrime, yPrime);
        float cosAlpha = (m_l2 * m_l2 + D * D - m_l3 * m_l3) / (2 * m_l2 * D);
        if (cosAlpha < -1.0 || cosAlpha > 1.0)
        {
            Serial.println("Error: invalid position cannot compute shoulder angle.");
            return;
        }
        float alpha = acos(cosAlpha);
        float theta2 = (phi - alpha) * (180.0 / M_PI);

        theta1 -= 75;
        theta2 += 90;
        theta3 = 180 - theta3;

        // Set joint angles
        m_base.setRotationSmooth(servoDriver, theta1);
        m_shoulder.setRotationSmooth(servoDriver, theta2);
        m_elbow.setRotationSmooth(servoDriver, theta3);

        // Debugging output
        Serial.println(theta1);
        Serial.println(theta2);
        Serial.println(theta3);
    }

    void resetRotations(Adafruit_PWMServoDriver servoDriver)
    {
        m_base.setRotation(servoDriver, 90);
        m_shoulder.setRotation(servoDriver, 90);
        m_elbow.setRotation(servoDriver, 90);
    }

    void setRotations(Adafruit_PWMServoDriver servoDriver, float base=90, 
        float shoulder=90, float elbow=90)
    {
        m_base.setRotationSmooth(servoDriver, base);
        m_shoulder.setRotationSmooth(servoDriver, shoulder);
        m_elbow.setRotationSmooth(servoDriver, elbow);
    }
};