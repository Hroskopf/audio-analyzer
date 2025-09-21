#ifndef REWINDBUTTON
#define REWINDBUTTON
#include "button.hpp"

// class for a left and right rewind buttons. Derived for a Button class
class RewindButton : public Button {

    // true if this is a left rewind button
    bool isLeft;

    // triangles shapes for the icon triangles
    sf::CircleShape leftTriangle, rightTriangle;

    // returns a triangle shapes for an icon of a button. r is a radius of circle (that defines the triangle), (x_, y_) is a coordinates of the center of circle
    sf::CircleShape create_triangle(float r, float x, float y) const {
        sf::CircleShape triangle;
        triangle.setPointCount(3);
        triangle.setRadius(r);
        triangle.setFillColor(IconColor);        
        triangle.setOrigin({r, r});
        triangle.setPosition({x, y});
        if(isLeft)
            triangle.rotate(sf::degrees(270));
        else {
            triangle.rotate(sf::degrees(90));
        }
        return triangle;
    }
    
public:
    // radius_ is a radius of button. (x_, y_) is coordinates of the top-left point of the square that defines circle. is_left is 0/1 wether it's a left rewind button
    RewindButton(float radius_, float x_, float y_, bool is_left = false) : Button(radius_, x_, y_, sf::Color::White, sf::Color({200, 200, 200})), isLeft(is_left) {

        leftTriangle = create_triangle(2 * radius / 3, x + 2.5 * radius / 3, y + radius);
        rightTriangle = create_triangle(2 * radius / 3, x + 4 * radius / 3, y + radius);

    }

    // draws the button on the given surface
    void draw(sf::RenderWindow& surface) {

        Button::draw(surface);
        
        surface.draw(leftTriangle);
        surface.draw(rightTriangle);

    }
};


#endif