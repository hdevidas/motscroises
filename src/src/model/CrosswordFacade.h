#ifndef CROSSWORDFACADE_H
#define CROSSWORDFACADE_H

#include "utils.h"
#include <vector>

/** @brief Facade used to group together all the functions that the outside would need to play our crossword games.*/
class CrosswordFacade
{
    public:
        virtual ~CrosswordFacade() {}

        /**
         * @brief Export function of the game, so that it can be imported later.
         * 
         * @param path Path to the folder where we want the export file to go.
         */
        virtual void exportGame(std::string path) = 0;

        /**
         * @brief Reveal word help function of the game, the specified word will be revealed if the player has enough points.
         * 
         * @param number Number of the word that we want to reveal.
         * @param horizontal Specify if this word is horizontal or vertical.
         * 
         * @return Returns 1 if the help was successful. Returns -1 if the player does not have enough points to use the help.
         */
        virtual int revealWord(unsigned number, bool horizontal) = 0;

        /**
         * @brief Reveal letter help function of the game, the specified letter will be revealed if the player has enough points.
         * 
         * @param x Column in the crossword grid of the letter.
         * @param y Row in the crossword grid of the letter.
         * 
         * @return Returns 1 if the help was successful. Returns -1 if the player does not have enough points to use the help.
         */
        virtual int revealLetter(unsigned x, unsigned y) = 0;

        /**
         * @brief Check word help function of the game, the player will be able to know if the word he has written corresponds to the solution.
         * 
         * @param number Number of the word that we want to reveal.
         * @param horizontal Specify if this word is horizontal or vertical.
         * 
         * @return Returns 1 if the word is good. Returns 0 if the word is bad. Returns -1 if the player does not have enough points to use the help.
         */
        virtual int checkWord(unsigned number, bool horizontal) = 0;

        /**
         * @brief Check letter help function of the game, the player will be able to know if the letter he has written corresponds to the solution.
         * 
         * @param x Column in the crossword grid of the letter.
         * @param y Row in the crossword grid of the letter.
         * 
         * @return Returns 1 if the letter is good. Returns 0 if the letter is bad. Returns -1 if the player does not have enough points to use the help.
         */
        virtual int checkLetter(unsigned x, unsigned y) = 0;

        /**
         * @brief Reveal grid function of the game, if the player is frustrated, he can reveal the whole grid.
         */
        virtual void revealGrid() = 0;

        /**
         * @brief Validate grid function of the game, if the player thinks he has succeeded in the game, he can validate the grid.
         * 
         * @return Returns true if the player grid is good, false if the player grid is bad.
         */
        virtual bool validateGrid() = 0;

        /**
         * @brief Reset grid function of the game, if the player wants to start this grid completely over, he can make all the squares white again.
         */
        virtual void resetGrid() = 0;

        /**
         * @brief Function that writes a letter in the player's grid.
         * 
         * @param x Column in the crossword grid of the letter.
         * @param y Row in the crossword grid of the letter.
         * @param letter Letter that will be write.
         */
        virtual void writeLetter(unsigned x, unsigned y, char letter) = 0;

        /**
         * @brief Function that delete a letter in the player's grid.
         * 
         * @param x Column in the crossword grid of the letter.
         * @param y Row in the crossword grid of the letter.
         */
        virtual void deleteLetter(unsigned x, unsigned y) = 0;

        /**
         * @return Returns a copy of the player's grid.
         */
        virtual std::vector<std::string> getCurrentGrid () = 0;

        /**
         * @param x Column in the crossword grid of the letter.
         * @param y Row in the crossword grid of the letter.
         * 
         * @return Returns the letter that is in the player's grid at the given position.
         */
        virtual char getLetter(unsigned x, unsigned y) = 0;

        /**
         * @return Returns all data associated with alls the words corresponding to the solution of the grid.
         */
        virtual std::vector<Word> getAllWords() = 0;

        /**
         * @param number Number of the word that we want to reveal.
         * @param horizontal Specify if this word is horizontal or vertical.
         * 
         * @return Returns all data associated with the specified word.
         */
        virtual Word getWord(unsigned number, bool horizontal) = 0;

        /**
         * @return Returns the remaining help points of the player.
         */
        virtual unsigned getRemainingHelpPoints() = 0;

        /**
         * @return Returns the height of the player's grid.
         */
        virtual unsigned getHeight() = 0;

        /**
         * @return Returns the width of the player's grid.
         */
        virtual unsigned getWidth() = 0;
};

#endif