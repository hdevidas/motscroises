#include "Difficulty.h"

/**
 * @brief Builds a difficulty object which will determine the characteristics (which are specific to the difficulty) of our crossword according to the given difficulty value.
 *
 * @param l The value of difficulty.
 */
Difficulty::Difficulty(Level l) {
    _level = l;
    if(l == EASY) {
        _heightOfTheGrid = 6;
        _widthOfTheGrid = 6;
        _name = "easy";
        _numberOfHelpPoints = 100;
    }
    else if(l == NORMAL) {
        _heightOfTheGrid = 8;
        _widthOfTheGrid = 8;
        _name = "normal";
        _numberOfHelpPoints = 80;
    }
    else if(l == HARD) {
        _heightOfTheGrid = 10;
        _widthOfTheGrid = 10;
        _name = "hard";
        _numberOfHelpPoints = 50;
    }
    else if(l == TEST) {
        _heightOfTheGrid = MAX_PATTERN_SIZE;
        _widthOfTheGrid = MAX_PATTERN_SIZE;
        _numberOfHelpPoints = 500;
    }
    else{
        throw DifficultyException();
    }
}

/**
 * @return Returns the width of the grid.
 */
unsigned Difficulty::getWidth() {
    return _widthOfTheGrid;
}

/**
 * @return Returns the height of the grid.
 */
unsigned Difficulty::getHeight(){
    return _heightOfTheGrid;
}

/**
 * @return Returns the litteral name of the difficulty.
 */
std::string Difficulty::getName(){
    return _name;
}

/**
 * @return Returns the starting number of help points.
 */
unsigned Difficulty::getNumberOfHelpPoints(){
    return _numberOfHelpPoints;
}

/**
 * @return Returns the value of difficulty.
 */
Level Difficulty::getLevel(){
    return this->_level;
}
