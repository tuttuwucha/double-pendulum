#pragma once
#include <cmath>
#include <atomic>

class Pendulum{
    public:
    std::atomic<double> poleLength = 1;
    std::atomic<double> poleAngle = 0;
    std::atomic<double> velocity = 0;
    std::atomic<double> acceleration = 0;
    std::atomic<double> mass = 1;
    
    void calculateVelocity(std::atomic<double>& delayMs){
        this->velocity = this->velocity + (this->acceleration * (delayMs / (double)1000));
    }

    void calculateAngle(std::atomic<double>& delayMs){
        this->poleAngle = this->poleAngle + (this->velocity * (delayMs / (double)1000));
    }

};