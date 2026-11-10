#pragma once
#include "Pendulum.h"

class SecondPendulum : public Pendulum{
    public:
    SecondPendulum(double PoleLength, double PoleAngle){
        poleLength = PoleLength;
        poleAngle = PoleAngle;
    }

    void calculateAcceleration(const Pendulum& first){
        const double G = 9.80665;
        this->acceleration = (double)(2 * std::sin(first.poleAngle - this->poleAngle) * (std::pow(first.velocity, 2) * first.poleLength * (first.mass + this->mass) + G * (first.mass + this->mass) * std::cos(first.poleAngle) + std::pow(this->velocity, 2) * this->poleLength * this->mass * std::cos(first.poleAngle - this->poleAngle))) / (double)(this->poleLength*(2*first.mass + this->mass - this->mass*std::cos(2*first.poleAngle - 2*this->poleAngle)));
    }

    double getX(const Pendulum& first){
        return first.poleLength * std::sin(first.poleAngle) + this->poleLength * std::sin(this->poleAngle);
    }

    double getY(const Pendulum& first){
        return first.poleLength * std::cos(first.poleAngle) + this->poleLength * std::cos(this->poleAngle);
    }
};
