#pragma once
#include "Pendulum.h"

class FirstPendulum : public Pendulum{
    public:
    FirstPendulum(double PoleLength, double PoleAngle){
        poleLength = PoleLength;
        poleAngle = PoleAngle;
    }

    void calculateAcceleration(const Pendulum& second){
        const double G = 9.80665;
        this->acceleration = (double)(-G*(2*this->mass + second.mass)*std::sin(this->poleAngle) - second.mass*G*std::sin(this->poleAngle - 2*second.poleAngle) - 2*std::sin(this->poleAngle - second.poleAngle) * second.mass * (std::pow(second.velocity, 2) * second.poleLength + std::pow(this->velocity, 2) * this->poleLength * std::cos(this->poleAngle - second.poleAngle))) / (double)(this->poleLength*(2*this->mass + second.mass - second.mass*std::cos(2*this->poleAngle - 2*second.poleAngle)));
    }

    double getX(){
        return this->poleLength * std::sin(this->poleAngle);
    }

    double getY(){
        return this->poleLength * std::cos(this->poleAngle);
    }
};
