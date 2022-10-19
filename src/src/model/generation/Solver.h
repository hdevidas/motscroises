#ifndef SOLVER_H
#define SOLVER_H

// #include "Lexicon.h"
#include "Pattern.h"
#include "Variable.h"
#include "AttributedWords.h"
#include <climits>
#include <chrono>

/** @brief Exception thrown when there is a PatternException during the solving.*/
struct SolverPatternException : public std::exception {
   /** @brief Exception message.*/
   const char * what () const throw () {
      return "SolverException: This is not a valid pattern for this solver : check pattern format in documentation.";
   }
};

/** @brief Exception thrown when there is a LexiconException during the solving.*/
struct SolverLexiconException : public std::exception {
   /** @brief Exception message.*/
   const char * what () const throw () {
      return "SolverException: This is not a valid lexicon for this solver : check lexicon format in documentation.";
   }
};

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

/** @brief This class contains the core of the algorithm for generating a crossword puzzle.*/
class Solver
{
    private:
        Pattern* _pattern; ///< Grid pattern that we want to solve
        Variable** _listVariables; ///< Variable set of the CSP
        AttributedWords* _attributedWords; ///< Keep an eye on variables that are already assigned
        unsigned _nbVariables = 0; ///< Total number of variables in the variable set
        char** _grid; ///< Grid on which the solving takes place

        void createVariablesAlgo();
        void verticalBrowse();
        void horizontalBrowse();
        void createNeighbours();
        Variable * chooseVariable();
        void updateGrid();
        void updateCell(Variable* var, char car, Position p, unsigned index);

    public:
        Solver(std::string pathToPattern);
        ~Solver();

        bool solve(); 
        char** getSolution();
        std::vector<Word> getWordVector();
        Pattern* getPattern();

        void printGrid();
        void displayAllVariables();
};
#endif
