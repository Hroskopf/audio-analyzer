#ifndef CONTROLLER
#define CONTROLLER

#include "GUIcomponents/all.hpp"

class WindowController {

    PlayButton playButton;
    ScrollBar scrollBar;
    RewindButton leftButton;
    RewindButton rightButton;
    GraphBox plot;
    sf::RenderWindow window;

    bool scroll_bar_was_clicked = false; // for checking if we clicked the scroll bar and holding the mouse


public:

    // Type for all events that are interesting for us
    enum EventType {
        PlayButtonClicked,
        LeftRewindButtonClicked,
        RightRewindButtonClicked,
        ScrollBarChange,
        SpaceClicked,
        LeftArrowClicked,
        RightArrowClicked,
        None,
    };

    WindowController(): playButton(40, 360, 510), scrollBar(600, 100, 490), leftButton(35, 260, 515, true), rightButton(35, 460, 515), plot(700, 400, 50, 20, -100, 0) {
        
        // set up higher resolution for the window
        sf::ContextSettings settings;
        settings.antiAliasingLevel = 16;
        
        // created main window with corresponding parameters
        window = sf::RenderWindow(sf::VideoMode({800, 600}), "AudioAnalyzer", sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed, settings);
    
    }

    // True if the main window is still open
    inline bool isOpen() const {
        return window.isOpen();
    }


    // returns an event as EventType member and deletes this event from a queue. 
    // This function is needed to be called untill it returns a None event
    EventType getEvent() {

        // Takes events via sf::RenderWindow::pollEvent until we get interesting for us event or we proccess all the events  
        while(const std::optional event = window.pollEvent())
        {
            // Window close
            if(event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Key click event
            if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

                // Space button is clicked
                if(keyPressed->scancode == sf::Keyboard::Scan::Space) {
                    playButton.change_state();
                    return SpaceClicked;
                }

                // Left arrow is clicked
                if(keyPressed->scancode == sf::Keyboard::Scan::Left) {
                    return LeftArrowClicked;
                }

                //Right arrow clicked
                if(keyPressed->scancode == sf::Keyboard::Scan::Right) {
                    return RightArrowClicked;
                }
            }   
                
            // Mouse button click
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    // Getting a mouse position on window
                    auto x = mouseButtonPressed->position.x;
                    auto y = mouseButtonPressed->position.y;

                    // Checking if play button is clicked
                    if(playButton.is_inside(x, y)) {
                        playButton.change_state();
                        return PlayButtonClicked;
                    }

                    // Checking for left rewind button click
                    if(leftButton.is_inside(x, y)) {
                        return LeftRewindButtonClicked;
                    }

                    // Checking for right rewind button click
                    if(rightButton.is_inside(x, y)) {
                        return RightRewindButtonClicked;
                    }

                    // Checking for scroll bar clicked. We will return a scrollbar click event, when the button will be released
                    if(scrollBar.is_inside(x, y)) {
                        scroll_bar_was_clicked = true;
                    }
                }
            }

            // Mouse button release
            if(const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
                if(mouseButtonReleased->button == sf::Mouse::Button::Left) {

                    // If button click begun at a scrollbar, we return a scrollbar click event 
                    if(scroll_bar_was_clicked) {                    
                        scroll_bar_was_clicked = false;
                        return ScrollBarChange;
                    }
                }
            }
        }

        // If we clicked on a scrollbar and still didn't release, we need to update an scrollbar position (without an audio rewind)
        auto [x, y] = sf::Mouse::getPosition(window);
        if(scroll_bar_was_clicked && scrollBar.is_inside(x, y)) {
            scrollBar.set_dot_position(x);
        }
        return None;
    }


    // Returns a position of a dot on a scrollbar (as a percent of a whole distance)
    inline float getBarPosition() const {
        return scrollBar.get_position_percent();
    }

    // setting of a scrollbar position as a percent of a distance
    inline void setBarPosition(const float percent) {

        // It is used to update a bar by current song duration. It must not change if the scrollbar position is being changed by a mouse.
        if(scroll_bar_was_clicked){
            return;
        }
        scrollBar.set_position_by_percent(percent);
    }

    // Sets a playing button state (playing/stopped) from a bool value (0 -> stopped, 1 -> playing)
    inline void setPlayingButtonState(const bool state) {
        playButton.set_state(state);
    }

    // Draw everything on the main window. plotHeights is an array of values needed to be shown on a graphBox
    void drawAndDisplay(const std::vector<double>&plotHeights) {
        
        // clear the window with a white color
        window.clear(sf::Color::White);

        // draw each component
        plot.draw(window, plotHeights);
        playButton.draw(window);
        scrollBar.draw(window);
        leftButton.draw(window);
        rightButton.draw(window);

        // display everything on a main window
        window.display();
    }

};

#endif