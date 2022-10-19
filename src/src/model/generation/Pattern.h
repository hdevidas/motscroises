#ifndef PATTERN_H
#define PATTERN_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define MIN_PATTERN_SIZE 3
#define MAX_PATTERN_SIZE 120

/** @brief Exception thrown the pattern file is not in the good format or corrupted.*/
struct PatternException : public std::exception {
    /** @brief Exception message.*/
   const char * what () const throw () {
      return "PatternException: verify the file is in the good format";
   }
};

/** @brief Used to create, from a txt file, a grid pattern that can be manipulated by our Solver.*/
class Pattern
{
    private:
        unsigned _height; ///< Height of the grid
        unsigned _width; ///< Height of the grid
        char ** _grid; ///< Grid created from the pattern

    public:
        Pattern(std::string pattern);
        ~Pattern();
        unsigned getWidth();
        unsigned getHeight();
        char ** getGrid();
};
#endif
