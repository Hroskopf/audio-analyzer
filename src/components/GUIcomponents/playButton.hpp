#ifndef PLAYBUTTON
#define PLAYBUTTON
#include "button.hpp"

// class for a play button. It is derived from a Button class
class PlayButton : public Button {
    
    // type for possible states of a button
    enum State {
        Stopped,
        Playing,
    } state;

    // in stopped state button icon looks like two vertical rectangles
    sf::RectangleShape leftRectangle, rightRectangle;
    // otherwise, it's a triangle
    sf::CircleShape triangle;

    // creates a rectangle with given parameters and iconColor. It is needed to define a leftRectangle and rightRectangle
    inline sf::RectangleShape create_icon_rect(const float height_, const float width_, const float x_, const float y_) const {
        sf::RectangleShape rect;
        rect.setSize({height_, width_});
        rect.setFillColor(IconColor);
        rect.setPosition({x_, y_});
        return rect;
    }

    // creates a sf::CircularShape which is a triangle icon in stopped state. (x_, y_) is coordinates of a center of a corresponding circle
    inline sf::CircleShape create_icon_triangle(const float radius_, const float x_, const float y_) const {
        sf::CircleShape triangle;
        triangle.setPointCount(3);
        triangle.setRadius(radius_);
        triangle.setFillColor(IconColor);        
        triangle.setOrigin({radius_, radius_});
        triangle.rotate(sf::degrees(90));
        triangle.setPosition({x_, y_});
        return triangle;
    }

    
public:
    // radius_ is a radius of a background circle, (x_, y_) is a coordinates of a left-top point of a square, that defines the background circle
    PlayButton(float radius_, float x_, float y_) : Button(radius_, x_, y_, sf::Color({200, 200, 200}), sf::Color::White) {

        state = Stopped;

        leftRectangle = create_icon_rect(radius / 3, radius, x + radius / 2, y + radius / 2);

        rightRectangle = create_icon_rect(radius / 3, radius, x + 2 * radius - radius / 2 - radius / 3, y + radius / 2);

        triangle = create_icon_triangle(2 * radius / 3, x + radius, y + radius);

    }

    // changes the current state of button
    inline void change_state() {
        if(state == Stopped) {
            state = Playing;
        }
        else {
            state = Stopped;
        }
    }

    // sets a state to a button from a bool value (1 -> playing, 0 -> stopped)
    inline void set_state(const bool new_state) {
        if(new_state) {
            state = Playing;
        }
        else {
            state = Stopped;
        }
    }

    // draws the button on the given surface
    void draw(sf::RenderWindow& surface) const {

        Button::draw(surface);
        
        if(state == Playing) {
            surface.draw(leftRectangle);
            surface.draw(rightRectangle);
        }
        else {
            surface.draw(triangle);
        }

    }
};


#endif