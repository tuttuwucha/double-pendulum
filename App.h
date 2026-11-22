#pragma once
#include "FirstPendulum.h"
#include "SecondPendulum.h"
#include "Utils.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <numbers>
#include <atomic>
#include <optional>

class App {
    public:

        

        sf::Vector2u resolution = {1920, 1080};
        double stepPerUnit = 200;
        

        App(){}

        App(unsigned int Width, unsigned int Height){
            resolution.x = Width;
            resolution.y = Height;
        }

        

        void draw(sf::RenderWindow& window, FirstPendulum& first, SecondPendulum& second){
            sf::CircleShape firstCircle(first.mass * (stepPerUnit / 10));
            firstCircle.setOrigin(firstCircle.getGeometricCenter());
            firstCircle.setFillColor(sf::Color::Red);
            sf::Vector2f firstPos = Utils::getPosFromCenterByXY(first.getX(), first.getY(), stepPerUnit, resolution);
            firstCircle.setPosition({firstPos.x, firstPos.y});


            sf::CircleShape secondCircle(second.mass * (stepPerUnit / 10));
            secondCircle.setOrigin(secondCircle.getGeometricCenter());
            secondCircle.setFillColor(sf::Color::Blue);
            sf::Vector2f secondPos = Utils::getPosFromCenterByXY(second.getX(first), second.getY(first), stepPerUnit, resolution);
            secondCircle.setPosition(secondPos);

            sf::VertexArray poles(sf::PrimitiveType::Lines, 4);
            sf::Vector2f center(this->resolution.x / 2.f, this->resolution.y / 2.f);
            poles[0].position = center;
            poles[1].position = firstPos;
            poles[2].position = firstPos;
            poles[3].position = secondPos;


            std::vector<sf::CircleShape> backgroundPoints = Utils::getBackgroundPoints(resolution, stepPerUnit);


            for(auto& point : backgroundPoints){
                window.draw(point);
            }
            window.draw(poles);
            window.draw(firstCircle);
            window.draw(secondCircle);
        }

        void run(){

            sf::RenderWindow window(sf::VideoMode({this->resolution.x, this->resolution.y}), "Double Pendulum");
            window.setFramerateLimit(100);

            FirstPendulum first(1, 3.14159);
            SecondPendulum second(1, 0);

            std::atomic<double> calculationDelayMs{1.0};

            std::jthread thread([&](std::stop_token stoken){
                auto nextStep = std::chrono::steady_clock::now();
                while(!stoken.stop_requested()){
                    
                    first.calculateAcceleration(second);
                    second.calculateAcceleration(first);

                    first.calculateVelocity(calculationDelayMs);
                    first.calculateAngle(calculationDelayMs);

                    second.calculateVelocity(calculationDelayMs);
                    second.calculateAngle(calculationDelayMs);

                    nextStep += std::chrono::milliseconds(calculationDelayMs);
                    if (std::chrono::steady_clock::now() > nextStep)
                    {
                        nextStep = std::chrono::steady_clock::now();
                    }   
                    std::this_thread::sleep_until(nextStep);
                }
            });
            
            
            
            while (window.isOpen()) {
                
                while (const std::optional event = window.pollEvent()) {
                    if(event->is<sf::Event::Closed>()){
                        window.close();
                    }
                    if (const auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>()){
                        if (wheel->wheel == sf::Mouse::Wheel::Vertical){
                            if (wheel->delta > 0){
                                this->stepPerUnit += 10;
                            }
                            else if(wheel->delta < 0){
                                if(!(this->stepPerUnit <= 50.0)){
                                    this->stepPerUnit -= 10;
                                }
                            }
                        }
                    }
                }
                
                window.clear();

                this->draw(window, first, second);

                window.display();

            }

            

        }

        

        
};