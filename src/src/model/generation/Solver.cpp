#include "Solver.h"

/**
 * @brief Builds the Solver object: creates the set of variables and the pattern in order to be able to run the solving.
 * 
 * @param pathToPattern Path to pattern txt file.
 */
Solver::Solver(std::string pathToPattern) {

    try{
        _pattern = new Pattern(pathToPattern);
    }catch(PatternException e){
        std::cerr << e.what() << std::endl;
        throw SolverPatternException();
    }

    _attributedWords = new AttributedWords();
    
    _grid = (char **)malloc(_pattern->getHeight() * sizeof(char *));
    for (unsigned i = 0; i < _pattern->getHeight(); i++) {
        _grid[i] = (char *)malloc(_pattern->getWidth() * sizeof(char));
        memcpy(_grid[i],_pattern->getGrid()[i], _pattern->getWidth() * sizeof(char));
    }

    try{
        createVariablesAlgo();
        createNeighbours();
    }catch(LexiconException e){
        std::cerr << e.what() << std::endl;
        throw SolverLexiconException();
    }
}

/**
 * @brief Destroys the Solver object
 * 
 */
Solver::~Solver() {
    for (unsigned i = 0; i < _pattern->getHeight(); i++){
        free(_grid[i]);
    }
    free(_grid);

    for (unsigned i = 0; i < _nbVariables; i++){
        delete _listVariables[i];
    }
    free(_listVariables);
    delete _attributedWords;
    delete _pattern;
}


/**
 *   @brief Print the grid.
*/
void Solver::printGrid() {
    updateGrid();
    std::cout << std::endl;
    for (unsigned i = 0; i < _pattern->getHeight(); i++) {
        for (unsigned j = 0; j < _pattern->getWidth(); j++){
            std::cout << _grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
   std::cout << std::endl;
}


/**
 *   @brief All values in the grid are updated with those contained in the Variables.
*/
void Solver::updateGrid() {
    Position p;
    char * word;
    for(unsigned indexVariable = 0 ; indexVariable < _nbVariables; indexVariable ++ ){
        if (_listVariables[indexVariable]->getWord() == NULL) {
            // #pragma omp parallel for schedule(static, 1)
            for(unsigned indexSize = 0; indexSize < _listVariables[indexVariable]->getSize(); indexSize++){
                p = _listVariables[indexVariable]->getPos();
                updateCell(_listVariables[indexVariable],'_',p,indexSize);
            }
        }
    }

    for(unsigned indexVariable = 0 ; indexVariable < _nbVariables; indexVariable ++ ){
        if (_listVariables[indexVariable]->getWord() != NULL) {
            // #pragma omp parallel for schedule(static, 1)
            for(unsigned indexSize = 0; indexSize < _listVariables[indexVariable]->getSize(); indexSize++){
                p = _listVariables[indexVariable]->getPos();
                word = _listVariables[indexVariable]->getWord();
                updateCell(_listVariables[indexVariable],word[indexSize],p,indexSize);
            }
      }
    }
}


/**
 *   @brief updateGrid() auxiliary function : updates the variable @p var at position @p p with the value @p car.
*/
void Solver::updateCell(Variable* var, char car, Position p, unsigned index){
    if (var->isHorizontal()){
        _grid[p.y][p.x + index] = car;
    }
    else {_grid[p.y +  index][p.x] = car;}
}


/**
 *   @brief Goes through the pattern horizontally and vertically detecting all the potential variables, creates these variables and adds them to the set.
*/
void Solver::createVariablesAlgo() {
    _listVariables = (Variable **) calloc(_pattern->getWidth() * _pattern->getHeight(), sizeof(Variable*));

    horizontalBrowse();
    verticalBrowse();
}


/**
 *   @brief createVariablesAlgo() auxiliary function : Browses the grid horizontally, creates the horizontal variables.
*/
void Solver::horizontalBrowse(){
    unsigned variableNumber = 0;
    unsigned variableLength = 0;
    char lastChar = '#';
    char currentChar = '#';
    unsigned posj = 0;
    unsigned posi = 0;
    Position p;
    Lexicon* database = Lexicon::getInstance(NULL);
    char ** grid = _pattern->getGrid();

    for (unsigned i = 0; i < _pattern->getHeight(); i++){
        for (unsigned j = 0; j < _pattern->getWidth(); j++){
            lastChar = currentChar;
            currentChar = grid[i][j];
            if (lastChar == '#' && currentChar == '_'){ // Starts a new variable
                variableLength++;
                posi = i;
                posj = j;
            }else if (lastChar == '_' && currentChar == '_'){ // Continue variable
                variableLength++;
            }else if (lastChar == '_' && currentChar == '#' && variableLength >= database->getLengthMin()){ // End of variable (variable added)
                variableNumber++;
                p.x = posj;
                p.y = posi;
                Variable* var = new Variable(true, variableLength, p);
                _listVariables[_nbVariables++] = var;
                variableLength = 0;
            }
            else if (lastChar == '_' && currentChar == '#'){  // End of variable (variable not added)
                variableLength = 0;
            }
        }
        if (currentChar == '_' && variableLength >= database->getLengthMin()){
            p.x = posj;
            p.y = posi;
            Variable* var = new Variable(true, variableLength, p);
            _listVariables[_nbVariables++] = var;
        }
        currentChar = '#';
        variableLength = 0;
    }
}


/**
 *   @brief createVariablesAlgo() auxiliary function : Browses the grid vertically, creates the vertical variables.
*/
void Solver::verticalBrowse(){
    unsigned variableNumber = 0;
    unsigned variableLength = 0;
    char lastChar = '#';
    char currentChar = '#';
    unsigned posj = 0;
    unsigned posi = 0;
    Position p;
    Lexicon* database = Lexicon::getInstance(NULL);
    char ** grid = _pattern->getGrid();

    for (unsigned j = 0; j < _pattern->getWidth(); j++){
        for (unsigned i = 0; i < _pattern->getHeight(); i++){
            lastChar = currentChar;
            currentChar = grid[i][j];

            if (lastChar == '#' && currentChar == '_'){
                variableLength++;
                posi = i;
                posj = j;
            }
            if (lastChar == '_' && currentChar == '_'){
                variableLength++;
            }
            if (lastChar == '_' && currentChar == '#' && variableLength >= database->getLengthMin()){
                variableNumber++;
                p.x = posj;
                p.y = posi;
                Variable* var = new Variable(false, variableLength, p);
                _listVariables[_nbVariables++] = var;
                variableLength = 0;
            }
            else if (lastChar == '_' && currentChar == '#'){
                variableLength = 0;
            }
        }
        if (currentChar == '_' && variableLength >= database->getLengthMin()){
            p.x = posj;
            p.y = posi;
            Variable* var = new Variable(false, variableLength, p);
            _listVariables[_nbVariables++] = var;
        }
        currentChar = '#';
        variableLength = 0;
    }  
}


/**
 * @brief For each variable in the set, add all its neighbours in the grid. 
 */
void  Solver::createNeighbours() {
    int pos;
    for (int i = 0; i < this->_nbVariables; i++) {        
        for (int j = i + 1; j < this->_nbVariables; j++) {
            pos = this->_listVariables[i]->intersectWith(this->_listVariables[j]);
            // If it intersects : add mutually the variables and their respective position
            if(pos != -1){
                int nPos = _listVariables[j]->intersectWith(this->_listVariables[i]);
                this->_listVariables[i]->addNeighboors(pos,this->_listVariables[j]);
                this->_listVariables[j]->addNeighboors(nPos,this->_listVariables[i]);
            }
        }
    }
}


/**
 *   @brief Solver auxiliary function : Choose the variable with the smallest domain.
 * 
 *   @return Returns the variable chose.
*/
Variable * Solver::chooseVariable() {
    int indexMin;
    for (int i = 0; i < this->_nbVariables; i++){
        if(_listVariables[i]->getWord() == NULL){
            indexMin = i;
            break;
        }
    }
    if(indexMin == _nbVariables-1){
        return _listVariables[indexMin];
    }  
    for (int index = indexMin+1; index < _nbVariables; index++){
        if(_listVariables[index]->getWord() != NULL){continue;} 
        if (_listVariables[index]->getDomain()->getDomainSize() < _listVariables[indexMin]->getDomain()->getDomainSize()){
            indexMin = index;
        }
    }
    return _listVariables[indexMin];
}


/**
 *   @brief Algorithm to solve a grid.
 * 
 *   @return Returns true if the grid is resoluble, false otherwise.
*/
bool Solver::solve() {
    bool solvable = true;

    std::cout << "Solving the grid..." << std::endl;
    auto t1 = high_resolution_clock::now(); //to record the duration time

    Variable** fillingOrder = (Variable **) malloc(_nbVariables * sizeof(Variable*)); //Initializes a Variable array to save the order in which the variables have been filled.

    unsigned varIndex = 0; // Each index represents a Variable, when a word inside a variable has been set, index increments
    unsigned btLevel = 0; // BackTracking level
    try{
        while(varIndex < _nbVariables) { //while all the grid is not filled
            Variable * var = chooseVariable(); //select the variable (the most difficult to insert)
            bool attributed = true; //to know if the word we'll select is already in the grid
            int wordIndex = -1;
            
            while(true){
                wordIndex = var->getDomain()->findWord(); //gets an index word from the domain variable
                if(wordIndex == -1){break;}
                attributed = _attributedWords->isAttributed(var->getSize(),wordIndex); // We look for a word that is not already in the grid
                if(!attributed){break;}
            }

            if(wordIndex!=-1) { // If we managed to assign this variable
                var->setWord(wordIndex); // Add the word to the Variable
                _attributedWords->addAttributedWord(var->getSize(),wordIndex); // The word is in the grid and will not be allowed to be placed again
                var->updateNeighbours(); // We update the neighboors's domain of the var that has been placed.
                fillingOrder[varIndex] = var;
                varIndex++; // word has been added
                btLevel = 0;
            } else { // We can't assign the variable --> we go back and hope to find a possible combination

                // if we can't assign the first variable anymore, it mean the grid is not resoluble
                if(varIndex == 0){
                    solvable = false;
                    break;
                }
                
                varIndex --; //Get the previous variable
                btLevel++;

                //if the previous variable has blocked a variable, we signal that we must no longer apply the same constraint : in order not to test words for nothing
                if(var->intersectWith(fillingOrder[varIndex]) != -1 && btLevel == 1)
                    var->conflict(fillingOrder[varIndex]);
                
                _attributedWords->removeAttributedWord(fillingOrder[varIndex]->getSize(),fillingOrder[varIndex]->getWordIndex());
                fillingOrder[varIndex]->reset(); //Reseting the previous variable : the next word in his domain will be choose next iteration
            }
            this->printGrid();
        }
    }catch(LexiconException e){
        std::cerr << e.what() << std::endl;
        throw SolverLexiconException();
    }
    auto t2 = high_resolution_clock::now(); //to record duration time

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    if(solvable)
        std::cout << "Grid completed in : " << ms_double.count() << "ms\n";   

    free(fillingOrder);

    updateGrid();
    return solvable;
}

/**
 * @brief Creates a list of all the useful data on the words in the grid for a crossword puzzle.
 * 
 * @return Returns a list Word.
 */
std::vector<Word> Solver::getWordVector(){
    std::vector<Word> words;
    unsigned number;
    std::string associateClue, word;
    bool horizontal;
    Position pos;
    int nextNumber = 1;
    int samePos = -1;
    for (int i = 0; i < this->_nbVariables; i++){
        horizontal = _listVariables[i]->isHorizontal();

        pos = _listVariables[i]->getPos();
        word = _listVariables[i]->getWord();
        associateClue = Lexicon::getAssociateClue(word);

        for(int j = 0; j < i; j++){
            if(_listVariables[i]->getPos().x == _listVariables[j]->getPos().x && _listVariables[i]->getPos().y == _listVariables[j]->getPos().y){
                samePos = j;
            }
        }
        if(samePos >= 0){
            number = words.at(samePos).number;
            samePos = -1;
        }
        else{
            number = nextNumber;
            nextNumber++;
        }
        Word w = {number,horizontal,pos,word,associateClue};
        words.push_back(w);
    }
    return words;
}

/**
 * @brief Prints the variable set.
 */
void Solver::displayAllVariables() {
    for (unsigned index =0; index < _nbVariables ; index ++){
        _listVariables[index]->printVariable();
    }
}

/**
 * @return Returns the solution grid.
 */
char** Solver::getSolution(){
    return this->_grid;
}

/**
 * @return Returns a reference to the grid pattern.
 */
Pattern* Solver::getPattern(){
    return this->_pattern;
}
