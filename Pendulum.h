#pragma once
#include <cmath>

class Pendulum{
    public:
    double poleLength = 1;
    double poleAngle = 0;
    double velocity = 0;
    double acceleration = 0;
    double mass = 1;
    
    void calculateVelocity(double delayMs){
        this->velocity = this->velocity + (this->acceleration * (delayMs / (double)1000));
    }

    void calculateAngle(double delayMs){
        this->poleAngle = this->poleAngle + (this->velocity * (delayMs / (double)1000));
    }

};