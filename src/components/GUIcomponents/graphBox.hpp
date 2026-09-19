#ifndef GRAPHBOX
#define GRAPHBOX
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>


// GUI component for graph visualising. Given array of heights is drawn like a bar chart.
class GraphBox {

    const float height, width; // the height and width of a background rectangle 
    const float x, y; // coordinates of a left-top point of a box
    const double max_value, min_value; // the maximum and minimum possible height of a bar (values of drawn array needs to be in range (0 ... max_value))
    const sf::Color GraphColor = sf::Color({166, 81, 176}); // the color of bar of the plot
    const sf::Color BackgroundColor = sf::Color({5, 5, 5}); // background ccolor of a plot
    const sf::Color LinesColor = sf::Color::White; // colors of lines of grid lines
    const sf::Color LettersColor = sf::Color::White; // collors of a letters of a text on a plot
    sf::RectangleShape backgroundRect; // background rectangle which is the whole graph box
    sf::RectangleShape graphAreaRect; // area on which the plot is drawn
    const size_t NumberOfGridLines = 9; // number of the horisontal grid lines shown on graph
    std::vector<sf::RectangleShape> gridLines; // vector of lines that needs to be drawn on screen
    std::vector<sf::Text> yLabels; // vector of text labels, written next to corresponding lines
    const float graph_area_x, graph_area_y, graph_area_height, graph_area_width; // coordinates (of the top-left) and dimentions of a plot area
    sf::Font font; // text font for a y labels

    // setting up parameters of a background rectangle
    void create_background_rect() {
        backgroundRect.setFillColor(BackgroundColor);
        backgroundRect.setPosition({x, y});
        backgroundRect.setOutlineThickness(2);
        backgroundRect.setOutlineColor(LinesColor);
    }

    // setting up rectangle for a graph area
    void create_graph_area_rect () {
        graphAreaRect.setSize({graph_area_width, graph_area_height});
        graphAreaRect.setFillColor(sf::Color(255, 255, 255, 0));
        graphAreaRect.setPosition({graph_area_x, graph_area_y});
        graphAreaRect.setOutlineThickness(2);
        graphAreaRect.setOutlineColor(LinesColor);
    }

    // set up grid lines and text labels figures to be drawn on the window
    void create_grid_lines_and_labels() {
        gridLines.resize(NumberOfGridLines);
        float space_between_grid_lines = graph_area_height / (NumberOfGridLines + 1);
        for(size_t i = 0;i < NumberOfGridLines;i++) {

            //creating a lines as a thin rectangles
            gridLines[i].setSize({graph_area_width, 1});
            gridLines[i].setPosition({graph_area_x, graph_area_y + (i + 1) * space_between_grid_lines});
            gridLines[i].setFillColor(LinesColor);

            // creating corresponding y-labels
            yLabels.push_back(sf::Text(font));
            yLabels[i].setPosition({graph_area_x - graph_area_width / 10, graph_area_y + (i + 1) * space_between_grid_lines - graph_area_height / 25});
            yLabels[i].setFillColor(LettersColor);
            yLabels[i].setCharacterSize(graph_area_height / 20);
            yLabels[i].setString(std::to_string(int(max_value - (max_value - min_value) * (i + 1) / (NumberOfGridLines + 1))));
        }
    }

public:
    // width_ and height_ are dimensions of the whole area. (x_, y_) are the coordinates of top-left point of an area. max_value_ and min_value_ are the biggest and smallest possible value of some bar.
    GraphBox(float width_, float height_, float x_, float y_, float min_value_, float max_value_): height(height_), width(width_), 
max_value(max_value_), min_value(min_value_),x(x_), y(y_), backgroundRect({width_, height_}), graph_area_x(x + width / 8), graph_area_y(y + height / 20), 
graph_area_height(85 * height / 100), graph_area_width(4 * width / 5), font("resources/fonts/DejaVuSans.ttf") {

        create_background_rect();

        create_graph_area_rect();

        create_grid_lines_and_labels();

    }
    
    // draw everything on a given surface and with given array of heights
    void draw(sf::RenderWindow& surface, const std::vector<double>& heights) const {

        // drawing the background rectangle
        surface.draw(backgroundRect);

        // draw the bars of the plot
        int number_of_blocks = heights.size();
        float bar_width = graph_area_width / number_of_blocks - 1;
        float bar_x = graph_area_x;
        for(auto block_height : heights) {
            block_height = std::min(block_height, max_value);
            block_height = std::max(block_height, min_value);
            float block_rect_height = ((block_height - min_value) / (max_value - min_value)) * graph_area_height; // height normalized to sizes of a graph_box
            sf::RectangleShape block({bar_width, block_rect_height});
            block.setPosition({bar_x, graph_area_y + graph_area_height - block_rect_height});
            block.setFillColor(GraphColor);
            bar_x += bar_width + 1;
            surface.draw(block);
        }

        // draw grid lines
        for(auto line : gridLines) {
            surface.draw(line);
        }

        // draw the labels 
        for(auto label : yLabels) {
            surface.draw(label);
        }
        
        // draw the graph area rectangle
        surface.draw(graphAreaRect);
    }

};


#endif