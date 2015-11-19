#pragma once

class PIDController {
public:
    const float kp, ki, kd;
    PIDController(float kp, float ki, float kd=0);

    // get the next output ofthe PID controller
    float update(float desired, float current);

    // amount to limit the integral term to, ie, saturation
    float integralLimit;

private:
    float integratedError;
};