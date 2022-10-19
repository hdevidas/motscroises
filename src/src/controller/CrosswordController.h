#ifndef CROSSWORDCONTROLLER_H
#define CROSSWORDCONTROLLER_H
#include "../model/CrosswordFacade.h"
#include "../view/MainWindow.h"
#include "../model/Crossword.h"

class MainWindow;
/** @brief This controller acts as a mediator between the graphical interface and the crossword game.*/
class CrosswordController{
    private:
        CrosswordFacade* _crossword = nullptr; ///< The state of the game
        MainWindow* _ui; ///< Reference to the graphical interface.
    public:
        CrosswordController(MainWindow* ui);
        ~CrosswordController();

        void createGame(Level diff);
        void importGame(std::string importPath);
        void exportGameAction(std::string exportPath);
        void letterChange(unsigned x, unsigned y, char c);
        void revealLetterAction(unsigned x, unsigned y);
        void revealWordAction(unsigned number, bool horizontal);
        void checkLetterAction(unsigned x, unsigned y);
        void checkWordAction(unsigned number, bool horizontal);
        void validateGridAction();
        void revealGridAction();
        void restartAction();
};

#endif