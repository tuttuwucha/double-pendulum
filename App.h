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

        static sf::Vector2f getPosByXY(sf::Vector2f center, double X, double Y){
            return sf::Vector2f(center.x + X * 200, center.y - Y * 200);
        }

        static void draw(sf::RenderWindow& window, FirstPendulum& first, SecondPendulum& second, unsigned int width, unsigned int height){
            sf::Vector2f center(width / 2.f, height / 2.f);

            sf::CircleShape firstCircle(first.mass * 20);
            firstCircle.setOrigin(firstCircle.getGeometricCenter());
            firstCircle.setFillColor(sf::Color::Red);
            sf::Vector2f firstPos = getPosByXY(center, first.getX(), first.getY());
            firstCircle.setPosition({firstPos.x, firstPos.y});
            

            sf::CircleShape secondCircle(second.mass * 20);
            secondCircle.setOrigin(secondCircle.getGeometricCenter());
            secondCircle.setFillColor(sf::Color::Blue);
            sf::Vector2f secondPos = getPosByXY(center, second.getX(first), second.getY(first));
            secondCircle.setPosition(secondPos);

            sf::VertexArray poles(sf::PrimitiveType::Lines, 4);
            poles[0].position = center;
            poles[1].position = firstPos;
            poles[2].position = firstPos;
            poles[3].position = secondPos;


            window.draw(poles);
            window.draw(firstCircle);
            window.draw(secondCircle);
        }

        void run(unsigned int width, unsigned int height){

            sf::RenderWindow window(sf::VideoMode({width, height}), "Double Pendulum");
            window.setFramerateLimit(100);

            FirstPendulum first(1, 3.14159);
            SecondPendulum second(2, 0);

            std::atomic<int> calculationDelayMs{1};

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

                this->draw(window, first, second, width, height);

                window.display();

            }

            

        }

        

        
};