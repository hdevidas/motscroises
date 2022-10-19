#ifndef CROSSWORD_H
#define CROSSWORD_H

#include "CrosswordFacade.h"
#include "Difficulty.h"
#include "generation/Pattern.h"
#include "generation/Solver.h"
#include <exception>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/** @brief Exception thrown when the crossword generation failed.*/
struct CrosswordGenerationException : public std::exception {
   /** @brief Exception message.*/
   const char * what () const throw () {
      return "CrosswordGenerationException: generation failure.";
   }
};

/** @brief Exception thrown when trying to access to a letter or word that is not in the crossword.*/
struct CrosswordBadAccessException : public std::exception {
   /** @brief Exception message.*/
   const char * what () const throw () {
      return "CrosswordBadAccessException: trying to get a word or letter that aren't in the crossword.";
   }
};

/** @brief Crossword is the class that holds the state of our game.*/
class Crossword : public CrosswordFacade
{
    private:
        Difficulty* _difficulty; ///< The difficulty of the game
        char** _currentGrid; ///< The state of the player's grid
        char** _solutionGrid; ///< The complete solution is in this grid
        std::vector<Word> _wordsInSolutionGrid; ///< All data about the words of the solution
        int _helpPoints; ///< Remaining help points of the player

        void verifyLetterAccess(unsigned x, unsigned y);
        
    public:
        Crossword(Level difficulty);
        Crossword(std::string importFile);
        Crossword(Level difficulty, std::string patternPath);
        ~Crossword();

        void exportGame(std::string path);    

        int revealWord(unsigned number, bool horizontal);
        int revealLetter(unsigned x, unsigned y);

        int checkWord(unsigned number, bool horizontal);
        int checkLetter(unsigned x, unsigned y);

        void revealGrid();
        bool validateGrid();
        void resetGrid();

        void writeLetter(unsigned x, unsigned y, char letter);
        void deleteLetter(unsigned x, unsigned y);

        std::vector<std::string> getCurrentGrid ();
        std::vector<std::string> getSolutionGrid ();
        char getLetter(unsigned x, unsigned y);
        std::vector<Word> getAllWords();
        Word getWord(unsigned number, bool horizontal);
        unsigned getRemainingHelpPoints();
        unsigned getHeight();
        unsigned getWidth();

        void printGrid(); //debug
        char** getCurrentGridTest ();
        char** getSolutionGridTest ();    
};
#endif
