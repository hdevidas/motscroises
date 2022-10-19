#ifndef DIFFICULTY_H
#define DIFFICULTY_H
#include <string>
#include "utils.h"
#include "generation/Pattern.h"

/** @brief Exception thrown when a non-existent difficulty is given.*/
struct DifficultyException : public std::exception {
    /** @brief Exception message.*/
   const char * what () const throw () {
      return "DifficultyException: this difficulty not exist";
   }
};

/** @brief Difficulty is the class that contains all the data associated with a difficulty of our crossword.*/
class Difficulty
{
    private:
        Level _level; ///< The value of difficulty
        std::string _name = ""; ///< Literal name of the difficulty
        unsigned _heightOfTheGrid; ///< Height of the grid involved in this difficulty
        unsigned _widthOfTheGrid; ///< Width of the grid involved in this difficulty
        unsigned _numberOfHelpPoints; ///< Number of starting help points involved in this difficulty
    public:
        Difficulty(Level l);
        unsigned getWidth();
        unsigned getHeight();
        std::string getName();
        unsigned getNumberOfHelpPoints();
        Level getLevel();
};
#endif
