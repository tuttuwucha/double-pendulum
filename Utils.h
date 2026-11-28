#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

class Utils {
public:
    static std::vector<sf::CircleShape> getBackgroundPoints(sf::Vector2u resolution, double step) {
        std::vector<sf::CircleShape> backgroundPoints;
        if (step <= 0.0) {
            return backgroundPoints;
        }

        sf::Vector2f center(resolution.x / 2.f, resolution.y / 2.f);
        float radius = static_cast<float>(step / 25.0);

        
        int stepsX = static_cast<int>(std::floor(center.x / step));
        int stepsY = static_cast<int>(std::floor(center.y / step));

        backgroundPoints.reserve((2 * stepsX + 1) * (2 * stepsY + 1));

        
        sf::CircleShape circleTemplate(radius);
        circleTemplate.setOrigin({radius, radius});
        circleTemplate.setFillColor(sf::Color({255, 255, 255, 50}));

        
        for (int gx = -stepsX; gx <= stepsX; ++gx) {
            for (int gy = -stepsY; gy <= stepsY; ++gy) {
                sf::CircleShape circle = circleTemplate;
                circle.setPosition(getPosFromCenterByXY(gx, gy, step, resolution));
                backgroundPoints.push_back(circle);
            }
        }

        return backgroundPoints;
    }

    static sf::Vector2f getPosFromCenterByXY(double X, double Y, double step, sf::Vector2u resolution) {
        double centerX = resolution.x / 2.0;
        double centerY = resolution.y / 2.0;
        return sf::Vector2f(
            static_cast<float>(centerX + X * step),
            static_cast<float>(centerY - Y * step)
        );
    }
};
