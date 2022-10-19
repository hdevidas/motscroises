#include "AttributedWords.h"

/**
 * @brief Construct an boolean array, there is a boolean for each word in the lexicon.
 */
AttributedWords::AttributedWords(){
    Lexicon* db = Lexicon::getInstance(NULL);
    for(unsigned i = 0; i < db->getLengthMax()+1; i++){
        bool* booltab = (bool*)malloc(db->getNbWords()*sizeof(bool));
        for(unsigned word = 0; word < db->getNbWords(); word++) booltab[word] = false;
        forbiddenWords.insert(std::pair<unsigned,bool*>(i,booltab));
    }
}

/**
 * @brief Destroys the AttributedWords object.
 * 
 */
AttributedWords::~AttributedWords(){
    for(unsigned i = 0; i < Lexicon::getInstance(NULL)->getLengthMax()+1; i++){
        free(forbiddenWords.at(i));
    }
    forbiddenWords.clear();
}

/**
 * @brief Set a word to 1 in the attributed words map.
 * 
 * @param size Size of the word we want to set to 1.
 * @param index Index in the lexicon of this word.
 */
void AttributedWords::addAttributedWord(unsigned size, unsigned index){
    forbiddenWords.at(size)[index] = true;
    
}

/**
 * @brief Set a word to 0 in the attributed word map.
 * 
 * @param size Size of the word we want to set to 0.
 * @param index Index in the lexicon of this word.
 */
void AttributedWords::removeAttributedWord(unsigned size, unsigned index){
    forbiddenWords.at(size)[index] = false;
}

/**
 * @brief Knowing whether a word is already attributed or not.
 * 
 * @param size Size of the word we want to get state.
 * @param index Index in the lexicon of this word.
 * 
 * @return Returns true if the word is attributed, false if is not.
 */
bool AttributedWords::isAttributed(unsigned size, unsigned index){
    return(forbiddenWords.at(size)[index]);
}
