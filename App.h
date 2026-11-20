#pragma once
#include "FirstPendulum.h"
#include "SecondPendulum.h"
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

        unsigned int width = 1920;
        unsigned int height = 1080;

        App(){}

        App(unsigned int Width, unsigned int Height){
            width = Width;
            height = Height;
        }

        sf::Vector2f getPosByXY(double X, double Y){
            sf::Vector2f center(this->width / 2.f, this->height / 2.f);
            return sf::Vector2f(center.x + X * 200, center.y - Y * 200);
        }

        void draw(sf::RenderWindow& window, FirstPendulum& first, SecondPendulum& second){
            sf::CircleShape firstCircle(first.mass * 20);
            firstCircle.setOrigin(firstCircle.getGeometricCenter());
            firstCircle.setFillColor(sf::Color::Red);
            sf::Vector2f firstPos = getPosByXY(first.getX(), first.getY());
            firstCircle.setPosition({firstPos.x, firstPos.y});
            

            sf::CircleShape secondCircle(second.mass * 20);
            secondCircle.setOrigin(secondCircle.getGeometricCenter());
            secondCircle.setFillColor(sf::Color::Blue);
            sf::Vector2f secondPos = getPosByXY(second.getX(first), second.getY(first));
            secondCircle.setPosition(secondPos);

            sf::VertexArray poles(sf::PrimitiveType::Lines, 4);
            sf::Vector2f center(this->width / 2.f, this->height / 2.f);
            poles[0].position = center;
            poles[1].position = firstPos;
            poles[2].position = firstPos;
            poles[3].position = secondPos;


            window.draw(poles);
            window.draw(firstCircle);
            window.draw(secondCircle);
        }

        void run(){

            sf::RenderWindow window(sf::VideoMode({this->width, this->height}), "Double Pendulum");
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
                }
                
                window.clear();

                this->draw(window, first, second);

                window.display();

            }

            

        }

        

        
};