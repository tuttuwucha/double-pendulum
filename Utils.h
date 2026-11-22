#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Utils
{
    public:

    static std::vector<sf::CircleShape> getBackgroundPoints(sf::Vector2u resolution, double step){
        std::vector<sf::CircleShape> backgroundPoints;
        sf::Vector2f center(resolution.x / 2.f, resolution.y / 2.f);
        float radius = step / 25.f;

        unsigned int temp = resolution.x;
        while(resolution.y >= step){
            
            while(resolution.x >= step){
                sf::CircleShape circle;
                circle.setRadius(radius);
                circle.setOrigin({radius, radius});
                circle.setFillColor(sf::Color({255, 255, 255, 50}));
                circle.setPosition({static_cast<float>(resolution.x), static_cast<float>(resolution.y)});
                backgroundPoints.push_back(circle);
                resolution.x -= step;
            }
            resolution.y -= step;
            resolution.x = temp;
        }

        return backgroundPoints;
    }

    static sf::Vector2f getPosFromCenterByXY(double X, double Y, double step, sf::Vector2u resolution){
        sf::Vector2f center(resolution.x / 2.f, resolution.y / 2.f);
        return sf::Vector2f(center.x + X * step, center.y - Y * step);
    }
};
