#ifndef UTILS_H
#define UTILS_H

#include <string>

/**
 * @file utils.h
 * 
 * @brief In this file we group enumerations and structures that are used in our code and that can be useful anywhere.
 * */

/** @brief Our crossword is a 2 dimension grid we use this type to store the position in the grid.*/
struct Position {
    unsigned x; ///< column
    unsigned y; ///< row
};

/**
 * @enum Level
 * @brief We use this enum to manipulate the differents crossword's difficulty.
 * */
enum Level {EASY, NORMAL, HARD, TEST};

/** @brief the "Crossword" word type. Use to represent what is a word in a Crossword game.*/
struct Word{
    unsigned number; ///< in crossword each words have a number
    bool horizontal; ///< true if horizontal, false if vertical
    Position pos; ///< position in the grid
    std::string word; ///< the word
    std::string associateClue; ///< the crossword clue associate to the word
};

/**
 * @enum HelpPointsCost
 * @brief This enum store the cost of each helps available in our crossword.
 * */
enum HelpPointsCost {CHECK_LETTER = 1, REVEAL_LETTER = 3, CHECK_WORD = 5, REVEAL_WORD = 10};

#endif