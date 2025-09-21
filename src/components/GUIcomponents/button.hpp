#ifndef BUTTON
#define BUTTON
#include <SFML/Graphics.hpp>

// Base class for all the buttons (rewind and play buttons)
class Button {
private:
    sf::CircleShape backgroudCircle;
    const sf::Color BackgroundColor;

protected:
    const float x, y, radius;
    const sf::Color IconColor;
    
public:

    // radius_ is a radius of a background circle, (x_, y_) - coordinated of a left-top dot of a square that defines a circle.
    Button(const float radius_, const float x_, const float y_, const sf::Color& backgroundColor, const sf::Color& iconColor) : x(x_), y(y_), radius(radius_), BackgroundColor(backgroundColor), IconColor(iconColor) {
        
        // set parameters for a background circle
        backgroudCircle.setRadius(radius);
        backgroudCircle.setPosition({x, y});
        backgroudCircle.setFillColor(BackgroundColor);
    }

    // this function is needs to be rewritten in derived classes. In this case it only draws a background circle
    virtual void draw(sf::RenderWindow& surface) const {
        surface.draw(backgroudCircle);
    }

    // returns if the given point's inside a button (is used for identifying if mouse click was on a button)
    inline bool is_inside(const float x_, const float y_) const {
        return (x - x_ + radius) * (x - x_ + radius) + (y - y_ + radius) * (y - y_ + radius) <= radius * radius;
    }

};


#endif