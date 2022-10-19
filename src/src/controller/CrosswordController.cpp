#include "CrosswordController.h"

/**
 * @brief Build the controller, just need a reference to the graphical interface.
 * 
 * @param ui Reference to the graphical interface.
 */
CrosswordController::CrosswordController(MainWindow* ui){
    this->_ui = ui;
}

/**
 * @brief Destroys the CrosswordController object.
 * 
 */
CrosswordController::~CrosswordController(){
    delete _crossword;
}

/**
 * @brief Create the crossword game.
 * 
 * @param diff Difficulty of the crossword.
 */
void CrosswordController::createGame(Level diff){
    try{
        Lexicon* lexicon = Lexicon::getInstance("../../data/databases/word_database.txt");
        this->_crossword = new Crossword(diff);
    }catch(CrosswordGenerationException e){
        std::cerr << e.what() << std::endl;
    }catch(std::invalid_argument& e){
        std::cerr << e.what() << std::endl;
    }
    this->_ui->initializeDisplay(_crossword->getHeight(),_crossword->getWidth(),_crossword->getCurrentGrid(),_crossword->getAllWords(),_crossword->getRemainingHelpPoints());
}

/**
 * @brief Import a crossword game.
 * 
 * @param importPath Path to the import file.
 */
void CrosswordController::importGame(std::string importPath){
    try{
        Lexicon* lexicon = Lexicon::getInstance("../../data/databases/word_database.txt");
        this->_crossword = new Crossword(importPath);
    }catch(CrosswordGenerationException e){
        std::cerr << e.what() << std::endl;
    }catch(std::invalid_argument& e){
        std::cerr << e.what() << std::endl;
    }
    this->_ui->initializeDisplay(_crossword->getHeight(),_crossword->getWidth(),_crossword->getCurrentGrid(),_crossword->getAllWords(),_crossword->getRemainingHelpPoints());
}

/**
 * @brief Export the crossword game.
 * 
 * @param exportPath Path to the export file.
 */
void CrosswordController::exportGameAction(std::string exportPath){
    this->_crossword->exportGame(exportPath);
    this->_ui->popup("Successfully saved.");
}

/**
 * @brief Manages the change of a letter.
 * 
 * @param x Column in the crossword grid of the letter.
 * @param y Row in the crossword grid of the letter.
 * @param c Letter that will be write.
 */
void CrosswordController::letterChange(unsigned x, unsigned y, char c){
    try{
        this->_crossword->writeLetter(x,y,c);
    }catch(CrosswordBadAccessException e){
        std::cerr << e.what() << std::endl;
    }
    _ui->refreshGrid(_crossword->getCurrentGrid());
}

/**
 * @brief Manages the request to reveal a letter.
 * 
 * @param x Column in the crossword grid of the letter.
 * @param y Row in the crossword grid of the letter.
 */
void CrosswordController::revealLetterAction(unsigned x, unsigned y){
    int success = -1;
    try{
        success = this->_crossword->revealLetter(x,y);
    }catch(CrosswordBadAccessException e){
        std::cerr << e.what() << std::endl;
    }
    if(success >= 0){
        this->_ui->refreshGrid(_crossword->getCurrentGrid());
        _ui->refreshHelpPoints(this->_crossword->getRemainingHelpPoints());
    }
    else
        this->_ui->popup("You don't have enough points to use this help.");
}

/**
 * @brief Manages the request to reveal a word.
 * 
 * @param number Number of the word that we want to reveal.
 * @param horizontal Specify if this word is horizontal or vertical.
 */
void CrosswordController::revealWordAction(unsigned number, bool horizontal){
    int success = -1;
    try{
        success = this->_crossword->revealWord(number,horizontal);
    }catch(CrosswordBadAccessException e){
        std::cerr << e.what() << std::endl;
    }
    if(success >= 0){
        this->_ui->refreshGrid(_crossword->getCurrentGrid());
        _ui->refreshHelpPoints(this->_crossword->getRemainingHelpPoints());
    }
    else
        this->_ui->popup("You don't have enough points to use this help.");
}

/**
 * @brief Manages the request to check a letter.
 * 
 * @param x Column in the crossword grid of the letter.
 * @param y Row in the crossword grid of the letter.
 */
void CrosswordController::checkLetterAction(unsigned x, unsigned y){
    int success = -1;
    try{
        success = this->_crossword->checkLetter(x,y);
    }catch(CrosswordBadAccessException e){
        std::cerr << e.what() << std::endl;
    }
    if(success >= 0){
        if(success)
            this->_ui->popup("This letter is good!");
        else
            this->_ui->popup("This letter is bad!");

        _ui->refreshHelpPoints(this->_crossword->getRemainingHelpPoints());
    }
    else
        this->_ui->popup("You don't have enough points to use this help.");
}

/**
 * @brief Manages the request to check a word.
 * 
 * @param number Number of the word that we want to reveal.
 * @param horizontal Specify if this word is horizontal or vertical.
 */
void CrosswordController::checkWordAction(unsigned number, bool horizontal){
    int success = -1;
    try{
        success = this->_crossword->checkWord(number, horizontal);
    }catch(CrosswordBadAccessException e){
        std::cerr << e.what() << std::endl;
    }
    if(success >= 0){
        if(success)
            this->_ui->popup("This word is good!");
        else
            this->_ui->popup("This word is bad!");

        _ui->refreshHelpPoints(this->_crossword->getRemainingHelpPoints());
    }
    else
        this->_ui->popup("You don't have enough points to use this help.");
}

/**
 * @brief Manages the request to validate the grid.
 */
void CrosswordController::validateGridAction(){
    bool success = _crossword->validateGrid();
    if(success)
        this->_ui->popup("Congratulations! You won!");
    else
        this->_ui->popup("Your grid is not currently the good one, try again !");
        
}

/**
 * @brief Manages the request to reveal the grid.
 */
void CrosswordController::revealGridAction(){
    this->_crossword->revealGrid();
    this->_ui->refreshGrid(_crossword->getCurrentGrid());
}

/**
 * @brief Manages the request to reset the grid.
 */
void CrosswordController::restartAction(){
    this->_crossword->resetGrid();
    this->_ui->refreshGrid(_crossword->getCurrentGrid());
}