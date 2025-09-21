#ifndef SCROLLBAR
#define SCROLLBAR
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

// class for a scroll bar
class ScrollBar {

    const float length; // length of a scrollbar
    const float x, y; // coordinates of the top-left point of a bar
    float dot_position; // current position of a dot
    const float dot_radius = 7; //radius of a dot
    const float line_width = 5; // width of the line
    sf::RectangleShape line; // line sf::Shape
    sf::CircleShape dot; // dot sf::Shape
    const sf::Color color = sf::Color({200, 200, 200}); // sf::Color of the bar


public:
    // length_ is a length of a bar (in pixels). (x_, y_) is a position of a top-left point of bar
    ScrollBar(float length_, float x_, float y_) : length(length_), x(x_), y(y_), dot_position(x) {
        dot.setOrigin({dot_radius, dot_radius});
        dot.setRadius(dot_radius);
        dot.setFillColor(color);

        line.setSize({length, line_width});
        line.setPosition({x, y + dot_radius - line_width / 2});
        line.setFillColor(color);

    }

    // draw bar on given surface
    void draw(sf::RenderWindow& surface) {
        dot.setPosition({dot_position, y + dot_radius});
        
        surface.draw(line);
        surface.draw(dot);
    }

    // true if given point is inside a bar (used for checking if mouse clicked a bar)
    bool is_inside(float x_, float y_) const {
        return (x - 100 <= x_ && x_ <= x + length + 100) && (y - 50 <= y_ && y_ <= y + 2 * dot_radius + 50);
    }

    // changes position of a dot by given new position in pixels (absolute position on screen)
    void set_dot_position(float new_position) {
        new_position = std::min(new_position, x + length);
        new_position = std::max(new_position, x);
        dot_position = new_position;
    }
    
    // changes position of a dot by given percent of the whole length
    void set_position_by_percent(float percent) {
        set_dot_position(x + (percent * length) / 100);
    }

    // returns a current position of a dot as a percent of length of bar
    float get_position_percent() const {
        return ((dot_position - x) / length) * 100;
    }

};


#endif