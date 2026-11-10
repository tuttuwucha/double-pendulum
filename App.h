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
        static void draw(sf::RenderWindow& window){
            
        }

        void run(unsigned int width, unsigned int height){

            sf::RenderWindow window(sf::VideoMode({width, height}), "Double Pendulum");
            window.setFramerateLimit(100);

            FirstPendulum first(1, 0.5);
            SecondPendulum second(1, 0);

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
                
                std::cout << "X: " << first.getX() << " Y: " << first.getY() << '\n';
                
                window.clear();
                window.display();

            }

            

        }

        

        
};