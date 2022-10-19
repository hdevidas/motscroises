#include "Variable.h"

/**
 * @brief Builds the base of a variable.
 *
 * @param horizontal True if the word is horizontal in the grid, 0 otherwise.
 * @param size The lenght of the variable.
 * @param pos Its pos in the grid (x,y).
 */
Variable::Variable(bool horizontal, unsigned size, Position pos) {
    this->_horizontal = horizontal;
    this->_word = NULL;
    this->_pos = pos;
    this->_size = size;
    this->_domain = new Domain(this);
}


/**
 * @brief Destroys the Variable object.
 * 
 */
Variable::~Variable() {
    _neighborhood.clear();
    free(_word);
    delete _domain;
}


/**
 * @brief Adds a neighboor to a variable.
 *
 * @param pos The position of intersection.
 * @param variable The variable that intersects.
 */
void Variable::addNeighboors(int pos, Variable* variable) {
    this->_neighborhood.insert(std::pair<Variable*,int>(variable,pos));
}


/**
 * @brief Removes a neighboor from a variable.
 * 
 * @param var Variable to remove.
 */
void Variable::removeNeighboors(Variable* var) {
    this->_neighborhood.erase(var);
}


/**
 * @brief Checks if @p variable intersects with 'this' (check only the pos).
 *
 * @param variable The variable to check.
 * 
 * @return Returns the pos in the variable of the letter that intersects with. Returns -1 otherwize.
 */ 
int Variable::intersectWith(Variable* variable){
    // Cannot intersect with a variable with the same orientation
    if(this->_horizontal == variable->_horizontal) {
        return -1;
    }
    int xThis = this->_pos.x;
    int yThis = this->_pos.y;
   
    for(int i = 0; i < this->_size; i++) {  // Loop through the pos of variable 'this'
        int xVar = variable->getPos().x;
        int yVar = variable->getPos().y;
        
        for(int j = 0; j < variable->getSize(); j++) { // Loop through the pos of the variable we want to check if it intersects with
            if(xThis == xVar && yThis == yVar){ // It intersects
                 return i;
            }
            // Update pos letter to check (variable)
            if(variable->isHorizontal()) { xVar++; }
            else { yVar++; }
        }
        // Update pos letter to check (this)
        if(this->_horizontal) { xThis++; }
        else { yThis++; }
    }
    return -1;
}


/**
 * @brief Prints the variable.
 */
void Variable::printVariable() {
    std::cout << "--- Variable ---" << std::endl;
    std::cout << "X : " << _pos.x << std::endl ;
    std::cout << "Y : " << _pos.y << std::endl ;
    if(_horizontal){
        std::cout << "Mot horizontal"<< std::endl ;
    }else{
        std::cout << "Mot vertical"<< std::endl ;
    }
    std::cout << "Size : " << _size<< std::endl ;
    if(_word != NULL) std::cout << "Word : " << _word << std::endl;
    std::cout << "Taille du domaine : " << getDomain()->getDomainSize() << std::endl ;
    std::cout << std::endl ;
}


/**
 * @brief Resets the value of the word to null and updates the neighbours domains accordingly (since the variable has become empty, it no longer applies constraints to its neighbours).
 */
void Variable::reset() {
    this->_word = NULL;
    this->_wordIndex = -1;
    this->_inReset = true;
    for (std::map<Variable*,int>::iterator it=_neighborhood.begin(); it!=_neighborhood.end(); ++it) {
        if (it->first->getWord() == NULL) {
            it->first->noMoreReseting();
        }
        it->first->getDomain()->initializeDomainValues();
        std::map<Variable*,int> neighboor_neighborhood = it->first->getNeighborhood();
        for (std::map<Variable*,int>::iterator it2=neighboor_neighborhood.begin(); it2!=neighboor_neighborhood.end(); ++it2) {
            if(it2->first->getWord() != NULL){
                it2->first->updateNeighbours(it->first); 
            }    
        }
    }
}

/**
 * @brief Applies the constraints (according to the value of the variable) to all its neighbours.
 * 
 * @param target If we ever want to apply the constraints on a specific variable.
 */
void Variable::updateNeighbours(Variable* target) {
    for (std::map<Variable*,int>::iterator it=_neighborhood.begin(); it!=_neighborhood.end(); ++it){
        Variable * neighboor = it->first;
        int pos = it->second;
        char letter = _word[pos];
        int mustHaveThisLetterAtPos = neighboor->getNeighborhood().at(this);
        if(target == NULL){
            neighboor->getDomain()->applyConstraint(letter,mustHaveThisLetterAtPos);
        }    
        else{
            if(target == neighboor){
                  neighboor->getDomain()->applyConstraint(letter,mustHaveThisLetterAtPos);
            }
        }      
    }
}

/**
 * @brief Optimization: retrieves the letter that blocked this variable and therefore tells the blocking variable to stop applying this constraint in the future (we avoid testing words that will apply the same "blocking" constraint).
 * 
 * @param var The blocking variable.
 */
void Variable::conflict(Variable* var){
    unsigned pos = var->getNeighborhood().at(this);
    char letter = var->getWord()[pos];
    var->getDomain()->discardWordsWithLetterAtPos(letter, pos);
}

/**
 * @return Returns the value of the variable.
 */
char* Variable::getWord() {
    return this->_word;
}

/**
 * @brief Changes the value of the variable from the index of a word (index in the Lexicon).
 * 
 * @param wordIndex The word index in the Lexicon.
 */
void Variable::setWord(unsigned wordIndex){
    this->_word = Lexicon::convertBitToWord(this->_domain->getWordInDomain(wordIndex),this->_size);
    this->_wordIndex = wordIndex;
}

/**
 * @return Returns the size of the variable.
 */
unsigned Variable::getSize(){ 
    return this->_size;
}

/**
 * @return Returns the Position of the variable.
 */
Position Variable::getPos() {
    return this->_pos;
}

/**
 * @return Returns the neighboors of the variable.
 */
std::map<Variable*,int> Variable::getNeighborhood() {

    return this->_neighborhood;
}

/**
 * @return Returns the word index (in the Lexicon) of the variable value.
 */
unsigned Variable::getWordIndex() {
    return this->_wordIndex;
}

/**
 * @brief Used to say that the variable is no longer being backtracked.
 */
void Variable::noMoreReseting() {
    this->_inReset = false;
}

/**
 * @return Returns true if the variable is horizontal in the grid. Returns false otherwise.
 */
bool Variable::isHorizontal() {
    return this->_horizontal;
}

/**
 * @return Returns true if the variable is undergoing backtracking. Returns false otherwise.
 */
bool Variable::isInReset() {
    return this->_inReset;
}

/**
 * @return Returns a reference to the variable domain.
 */
Domain* Variable::getDomain() {
    return this->_domain;
}