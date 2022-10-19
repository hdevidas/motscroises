#include "Crossword.h"

/**
 * @brief Build the game state from a difficulty only, the grid pattern is randomly chosen according to the difficulty.
 * 
 * @param difficulty Value of the difficulty.
 */
Crossword::Crossword(Level difficulty){
    try{
        _difficulty = new Difficulty(difficulty);
    }catch(DifficultyException e){
        std::cerr << e.what() << std::endl;
        throw CrosswordGenerationException();
    }

    _helpPoints = _difficulty->getNumberOfHelpPoints();

    _currentGrid = (char **)malloc(_difficulty->getHeight() * sizeof(char *));
    for (unsigned i = 0; i < _difficulty->getHeight(); i++) {
        _currentGrid[i] = (char *)malloc(_difficulty->getWidth() * sizeof(char));
    }

    _solutionGrid = (char **)malloc(_difficulty->getHeight() * sizeof(char *));
    for (unsigned i = 0; i < _difficulty->getHeight(); i++) {
        _solutionGrid[i] = (char *)malloc(_difficulty->getWidth() * sizeof(char));
    }

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,10);
    std::string pathToPattern = "../../data/patterns/" + _difficulty->getName() + "/"+_difficulty->getName() + std::to_string(dist6(rng)) + ".txt";

    Solver* solver = nullptr;
    try{
        solver = new Solver(pathToPattern);
    }catch(SolverLexiconException e){
        std::cerr << e.what() << std::endl;
        throw CrosswordGenerationException();
    }catch(SolverPatternException e){
        std::cerr << e.what() << std::endl;
        throw CrosswordGenerationException();
    }

    if(solver->getPattern()->getHeight() > _difficulty->getHeight() || solver->getPattern()->getWidth() > _difficulty->getWidth()) throw CrosswordGenerationException();
    for (unsigned i = 0; i < solver->getPattern()->getHeight(); i++) {
        memcpy(_currentGrid[i],solver->getPattern()->getGrid()[i], solver->getPattern()->getWidth() * sizeof(char));
    }

    if(!solver->solve()) throw CrosswordGenerationException();
    char** solution = solver->getSolution();
    for (unsigned i = 0; i < solver->getPattern()->getHeight(); i++) {
        memcpy(_solutionGrid[i],solution[i], solver->getPattern()->getWidth() * sizeof(char));
    }
    _wordsInSolutionGrid = solver->getWordVector();

    delete solver;
}

/**
 * @brief Build the state of the game by specifying the pattern on which you want to play.
 * 
 * @param difficulty Value of the difficulty.
 * @param patternPath Path to the pattern file.
 */
Crossword::Crossword(Level difficulty, std::string patternPath){
    try{
        _difficulty = new Difficulty(difficulty);
    }catch(DifficultyException e){
        std::cerr << e.what() << std::endl;
        throw CrosswordGenerationException();
    }
    _helpPoints = _difficulty->getNumberOfHelpPoints();

    _currentGrid = (char **)malloc(_difficulty->getHeight() * sizeof(char *));
    for (unsigned i = 0; i < _difficulty->getHeight(); i++) {
        _currentGrid[i] = (char *)malloc(_difficulty->getWidth() * sizeof(char));
    }

    _solutionGrid = (char **)malloc(_difficulty->getHeight() * sizeof(char *));
    for (unsigned i = 0; i < _difficulty->getHeight(); i++) {
        _solutionGrid[i] = (char *)malloc(_difficulty->getWidth() * sizeof(char));
    }

    Solver* solver = nullptr;
    try{
        solver = new Solver(patternPath);
    }catch(SolverLexiconException e){
        std::cerr << e.what() << std::endl;
        throw CrosswordGenerationException();
    }catch(SolverPatternException e){
        std::cerr << e.what() << std::endl;
        throw CrosswordGenerationException();
    }

    if(solver->getPattern()->getHeight() > _difficulty->getHeight() || solver->getPattern()->getWidth() > _difficulty->getWidth()) throw std::invalid_argument("Bad pattern input file");
    for (unsigned i = 0; i < solver->getPattern()->getHeight(); i++) {
        memcpy(_currentGrid[i],solver->getPattern()->getGrid()[i], solver->getPattern()->getWidth() * sizeof(char));
    }

    if(!solver->solve()) throw CrosswordGenerationException();
    char** solution = solver->getSolution();
    for (unsigned i = 0; i < solver->getPattern()->getHeight(); i++) {
        memcpy(_solutionGrid[i],solution[i], solver->getPattern()->getWidth() * sizeof(char));
    }
    _wordsInSolutionGrid = solver->getWordVector();

    delete solver;
}

/**
 * @brief Builds the game state from an import file.
 * 
 * @param importFile Path to the import file.
 */
Crossword::Crossword(std::string importFile){

    if(importFile.substr(importFile.find_last_of(".") + 1) != "json") throw std::invalid_argument("Import file must be in json");

    std::fstream file;
    file.open(importFile,std::ios::in);
    if (!file.is_open()) {
        throw std::invalid_argument("Import path doesn't exist");
    }

    json game;
    file >> game;

    try{
        _difficulty = new Difficulty(game["difficulty"]);
    }catch(DifficultyException e){
        std::cerr << e.what() << std::endl;
        throw CrosswordGenerationException();
    }
    _currentGrid = (char **)malloc(_difficulty->getHeight() * sizeof(char *));
    for (unsigned i = 0; i < _difficulty->getHeight(); i++) {
        _currentGrid[i] = (char *)malloc(_difficulty->getWidth() * sizeof(char));
    }

    _solutionGrid = (char **)malloc(_difficulty->getHeight() * sizeof(char *));
    for (unsigned i = 0; i < _difficulty->getHeight(); i++) {
        _solutionGrid[i] = (char *)malloc(_difficulty->getWidth() * sizeof(char));
    }

    for(int i = 0; i < getHeight(); i++){
        std::string line = game["current_grid"][i].get<std::string>();
        for(int j = 0; j < getWidth(); j++){
            _currentGrid[i][j] = line[j];
        }
    }

    for(int i = 0; i < getHeight(); i++){
        std::string line = game["solution_grid"][i].get<std::string>();
        for(int j = 0; j < getWidth(); j++){
            _solutionGrid[i][j] = line[j];
        }
    }

    for(int i = 0; i < game["words"].size(); i++){
        Word word = {game["words"][i]["number"],game["words"][i]["horizontal"],{game["words"][i]["pos"]["x"],game["words"][i]["pos"]["y"]},game["words"][i]["word"],game["words"][i]["associate_def"]};;
        _wordsInSolutionGrid.push_back(word);
    }

    _helpPoints = game["help_points"];

    file.close();
}

/**
 * @brief Destroys the Crossword object.
 * 
 */
Crossword::~Crossword(){
    for (unsigned i = 0; i < _difficulty->getHeight(); i++){
        free(_currentGrid[i]);
    }
    free(_currentGrid);

    for (unsigned i = 0; i < _difficulty->getHeight(); i++){
        free(_solutionGrid[i]);
    }
    free(_solutionGrid);

    _wordsInSolutionGrid.clear();
    
    delete _difficulty;
}

/**
 * @brief Check that we are not trying to access a non-existent position in the grid.
 * 
 * @param x Column in the crossword grid of the letter.
 * @param y Row in the crossword grid of the letter.
 */
void Crossword::verifyLetterAccess(unsigned x, unsigned y){
    if(y >= getHeight() || y < 0 || x >= getWidth() || x < 0) 
        throw CrosswordBadAccessException();
}

/**
 * @brief Export function of the game, so that it can be imported later.
 * 
 * @param path Path to the folder where we want the export file to go.
 */
void Crossword::exportGame(std::string path){
    if(path.substr(path.find_last_of(".") + 1) != "json") throw std::invalid_argument("Export file must be in json");

    json game;
    game["difficulty"] = _difficulty->getLevel();

    for(int i = 0; i < getHeight(); i++){
        std::string line(_currentGrid[i], getWidth());
        game["current_grid"][i] = line;
    }
    for(int i = 0; i < getHeight(); i++){
        std::string line(_solutionGrid[i], getWidth());
        game["solution_grid"][i] = line;
    }
    for(int i = 0; i < _wordsInSolutionGrid.size(); i++){
        Word w = _wordsInSolutionGrid.at(i);
        game["words"][i] = { {"number",w.number}, {"word", w.word}, {"horizontal", w.horizontal}, {"pos", {{"x", w.pos.x}, {"y", w.pos.y}}}, {"associate_def", w.associateClue}};
    }
    game["help_points"] = _helpPoints;

    std::fstream file;
    file.open(path,std::ios::out);
    if (!file.is_open()) {
        throw std::invalid_argument("Export path doesn't exist");
    }

    file << game;
    file.close();
}

/**
 * @brief Reveal word help function of the game, the specified word will be revealed if the player has enough points.
 * 
 * @param number Number of the word that we want to reveal.
 * @param horizontal Specify if this word is horizontal or vertical.
 * 
 * @return Returns 1 if the help was successful. Returns -1 if the player does not have enough points to use the help.
 */
int Crossword::revealWord(unsigned number, bool horizontal){
    int r = -1;
    if(_helpPoints - REVEAL_WORD >= 0){
        _helpPoints -= REVEAL_WORD;
        r = 1;
        Word wordToCheck = getWord(number, horizontal);
        int inc;
        wordToCheck.horizontal ? inc = wordToCheck.pos.x : inc = wordToCheck.pos.y;
        for(int i = inc; i <= inc + wordToCheck.word.size()-1; i++){
            wordToCheck.horizontal ? _currentGrid[wordToCheck.pos.y][i] = _solutionGrid[wordToCheck.pos.y][i] : _currentGrid[i][wordToCheck.pos.x] = _solutionGrid[i][wordToCheck.pos.x];
        }
    }
    return r;
}

/**
 * @brief Reveal letter help function of the game, the specified letter will be revealed if the player has enough points.
 * 
 * @param x Column in the crossword grid of the letter.
 * @param y Row in the crossword grid of the letter.
 * 
 * @return Returns 1 if the help was successful. Returns -1 if the player does not have enough points to use the help.
 */
int Crossword::revealLetter(unsigned x, unsigned y){
    verifyLetterAccess(x,y);
    int r = -1;
    if(_helpPoints - REVEAL_LETTER >= 0){
        _helpPoints -= REVEAL_LETTER;
        _currentGrid[y][x] = _solutionGrid[y][x];
        r = 1;
    }
    return r;
}

/**
 * @brief Check word help function of the game, the player will be able to know if the word he has written corresponds to the solution.
 * 
 * @param number Number of the word that we want to reveal.
 * @param horizontal Specify if this word is horizontal or vertical.
 * 
 * @return Returns 1 if the word is good. Returns 0 if the word is bad. Returns -1 if the player does not have enough points to use the help.
 */
int Crossword::checkWord(unsigned number, bool horizontal){
    int r = -1;
    if(_helpPoints - CHECK_WORD >= 0){
        _helpPoints -= CHECK_WORD;
        r = 1;
        Word wordToCheck = getWord(number, horizontal);
        int inc;
        wordToCheck.horizontal ? inc = wordToCheck.pos.x : inc = wordToCheck.pos.y;
        for(int i = inc; i <= inc + wordToCheck.word.size()-1; i++){
            if(wordToCheck.horizontal){
                if(_currentGrid[wordToCheck.pos.y][i] != _solutionGrid[wordToCheck.pos.y][i]) r = 0;
            }  
            else{
                if(_currentGrid[i][wordToCheck.pos.x] != _solutionGrid[i][wordToCheck.pos.x]) r = 0;
            }
        }
    }
    return r;
}

/**
 * @brief Check letter help function of the game, the player will be able to know if the letter he has written corresponds to the solution.
 * 
 * @param x Column in the crossword grid of the letter.
 * @param y Row in the crossword grid of the letter.
 * 
 * @return Returns 1 if the letter is good. Returns 0 if the letter is bad. Returns -1 if the player does not have enough points to use the help.
 */
int Crossword::checkLetter(unsigned x, unsigned y){
    verifyLetterAccess(x,y);
    int r = -1;
    if(_helpPoints - CHECK_LETTER >= 0){
        _helpPoints -= CHECK_LETTER;
        if(_solutionGrid[y][x] == _currentGrid[y][x])
            r = 1;
        else
            r = 0;
    }
    return r;
}

/**
 * @brief Reveal grid function of the game, if the player is frustrated, he can reveal the whole grid.
 */
void Crossword::revealGrid(){
    for (unsigned i = 0; i < getHeight(); i++) {
        memcpy(_currentGrid[i],_solutionGrid[i], getWidth() * sizeof(char));
    }
}

/**
 * @brief Validate grid function of the game, if the player thinks he has succeeded in the game, he can validate the grid.
 * 
 * @return Returns true if the player grid is good, false if the player grid is bad.
 */
bool Crossword::validateGrid(){
    bool valid = true;
    for(int i = 0; i < getHeight(); i++){
        for(int j = 0; j < getWidth(); j++){
            if(getLetter(j,i) != _solutionGrid[i][j])
                valid = false;
        }
    }
    return valid;
}

/**
 * @brief Reset grid function of the game, if the player wants to start this grid completely over, he can make all the squares white again.
 */
void Crossword::resetGrid(){
    for(int i = 0; i < getHeight(); i++){
        for(int j = 0; j < getWidth(); j++){
            if(getLetter(j,i) != '#')
                this->_currentGrid[i][j] = '_';
        }
    }
}

/**
 * @brief Function that writes a letter in the player's grid.
 * 
 * @param x Column in the crossword grid of the letter.
 * @param y Row in the crossword grid of the letter.
 * @param letter Letter that will be write.
 */
void Crossword::writeLetter(unsigned x, unsigned y, char letter){
    verifyLetterAccess(x,y);
    if (letter - 'A' > 26 || letter - 'A' < 0 )
        this->_currentGrid[y][x] = '_';
    else
        this->_currentGrid[y][x] = letter;

}

/**
 * @brief Function that delete a letter in the player's grid.
 * 
 * @param x Column in the crossword grid of the letter.
 * @param y Row in the crossword grid of the letter.
 */
void Crossword::deleteLetter(unsigned x, unsigned y){
    verifyLetterAccess(x,y);
    this->_currentGrid[y][x] = '_';
}

/**
 * @return Returns a copy of the player's grid.
 */
std::vector<std::string> Crossword::getCurrentGrid (){
    std::vector<std::string> grid;
    for(int i = 0; i < getHeight(); i++){
        std::string line;
        for(int j = 0; j < getWidth(); j++){
            line.insert(j,1,this->_currentGrid[i][j]);
        }
        grid.push_back(line);
    }
    return grid;
}

/**
 * @param x Column in the crossword grid of the letter.
 * @param y Row in the crossword grid of the letter.
 * 
 * @return Returns the letter that is in the player's grid at the given position.
 */
char Crossword::getLetter(unsigned x, unsigned y){
    verifyLetterAccess(x,y);
    return this->_currentGrid[y][x];
}

/**
 * @return Returns all data associated with alls the words corresponding to the solution of the grid.
 */
std::vector<Word> Crossword::getAllWords(){
    return this->_wordsInSolutionGrid;
}

/**
 * @param number Number of the word that we want to reveal.
 * @param horizontal Specify if this word is horizontal or vertical.
 * 
 * @return Returns all data associated with the specified word.
 */
Word Crossword::getWord(unsigned number, bool horizontal){
    for (unsigned i=0; i<_wordsInSolutionGrid.size(); i++){
        Word currentWord = _wordsInSolutionGrid.at(i);
        if(currentWord.number == number && currentWord.horizontal == horizontal)
            return currentWord;
    }
    throw CrosswordBadAccessException();
}

/**
 * @return Returns the remaining help points of the player.
 */
unsigned Crossword::getRemainingHelpPoints(){
    return this->_helpPoints;
}

/**
 * @return Returns the height of the player's grid.
 */
unsigned Crossword::getHeight(){
    return this->_difficulty->getHeight();
}

/**
 * @return Returns the width of the player's grid.
 */
unsigned Crossword::getWidth(){
    return this->_difficulty->getWidth();
}

/**
 *   @brief Only used for developping
*/
void Crossword::printGrid() {
    std::cout << std::endl;
    for (unsigned i = 0; i < getHeight(); i++) {
        for (unsigned j = 0; j < getWidth(); j++){
            std::cout << _currentGrid[i][j] << " ";
        }
        std::cout << std::endl;
    }
   std::cout << std::endl;
}

char** Crossword::getCurrentGridTest () {
    return _currentGrid;
}
char** Crossword::getSolutionGridTest () {
    return _solutionGrid;
}