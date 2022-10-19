#include "Domain.h"

/**
 * @brief Constructs the domain from the variable that owns it.
 */
Domain::Domain(Variable* var) {
    this->_variable = var;

    Lexicon* database = Lexicon::getInstance(NULL);
    this->_domainMaxSize = database->getMaxWordsForSize(_variable->getSize());
    
    this->_domain = (std::bitset<26> **) malloc(_domainMaxSize * sizeof(std::bitset<26> *));
    for (unsigned j = 0; j < _domainMaxSize; j ++) {
       this->_domain[j] = (std::bitset<26> *) malloc((database->getLengthMax()+1)* sizeof(std::bitset<26>));
    }

    this->initializeDomainValues();
}

/**
 * @brief Destroys the Domain object.
 * 
 */
Domain::~Domain() {

    for (unsigned j = 0; j < _domainMaxSize; j ++){
        free(this->_domain[j]);
    }
    free(this->_domain);

    _domainIndexList.clear();
}

/**
 * @brief Initializes the domain values by taking all the words in the lexicon that are of the size corresponding to the variable size.
 */
void Domain::initializeDomainValues() {
    Lexicon* database = Lexicon::getInstance(NULL);

    std::bitset<26>** domainToCopy= database->getWordByLenght(_variable->getSize());

    for (unsigned j = 0; j < _domainMaxSize; j ++) {
        memcpy(this->_domain[j],domainToCopy[j],(database->getLengthMax()+1) * sizeof(std::bitset<26>));
    }

    this->_domainSize = this->_domainMaxSize;

    bool variableInReset = this->_variable->isInReset();
    if(!variableInReset){
        _domainIndexList.clear();
        for(int i = 0; i < this->_domainMaxSize; i++) _domainIndexList.push_back(i);
        this->shuffleDomain();
    }
}


/**
 * @brief Apply a constraint to our domain, we only keep words with @p c at position @p pos .
 * 
 * @param c The character that we must have at position @p pos.
 * @param pos Letter pos in the variable.
 */
void Domain::applyConstraint(char c, unsigned pos) {
    bool variableInReset = this->_variable->isInReset();
    unsigned size = this->_variable->getSize();
    if(!variableInReset)_domainIndexList.clear();

    std::bitset<26> bitC;
    bitC.set(c - 'A');

    this->_domainSize = 0;
    for (unsigned currentWord = 0; currentWord < _domainMaxSize; currentWord ++){
        if (_domain[currentWord][pos] != bitC) {
           for(int i = 0; i < size; i++) _domain[currentWord][i].reset();
        }
        else{
            this->_domainSize++;
            if(!variableInReset)_domainIndexList.push_back(currentWord);
        }
    }

    if(!variableInReset)this->shuffleDomain();
}

/**
 * @brief Shuffle the list of indexes in order to add randomness to the generation, so as not to always generate the same grids.
 */
void Domain::shuffleDomain() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(_domainIndexList.begin(), _domainIndexList.end(), g);
}

/**
 * @brief Remove, from the domain, all words that have character @p c at position @p pos .
 * 
 * @param c The character.
 * @param pos Letter pos in the variable.
 */
void Domain::discardWordsWithLetterAtPos(char letter, unsigned pos){
    unsigned size = this->_variable->getSize();
    std::bitset<26> bitC;
    bitC.set(letter - 'A');
    for (unsigned i=0; i<_domainIndexList.size(); i++){
        if (_domain[_domainIndexList.at(i)][pos] == bitC) {
            _domainIndexList.erase(_domainIndexList.begin() + i);
        }
    }
    
}

/**
 * @brief Function that gives the next word in the domain that can be placed on the grid.
 * 
 * @return Returns the index of the word found, returns -1 if there are no more words that can be attributed.
 */
int Domain::findWord() {

    if(_domainIndexList.empty())
        return -1;

    unsigned wordIndex = _domainIndexList.front();
    _domainIndexList.erase(_domainIndexList.begin());
    
    return wordIndex;
}

/**
 * @return Returns the domain size.
 */
unsigned Domain::getDomainSize() {
    return this->_domainSize;
}

/**
 * @brief Get the word in the domain that is at @p wordIndex .
 * 
 * @param wordIndex Index of the word we want to get.
 * 
 * @return Returns the word, encoded in binary.
 */
std::bitset<26>* Domain::getWordInDomain(unsigned wordIndex) {
    return this->_domain[wordIndex];
}