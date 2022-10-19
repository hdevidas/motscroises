#include "Lexicon.h"

Lexicon* Lexicon::_singleton= nullptr;

/**
 * @brief Construct a new Lexicon object by binary encoding the words of a text file representing the database.
 * 
 * @param lexicon Contains the string words to be stored in bits.
 */
Lexicon::Lexicon(const char * lexicon) {
    _nbWords = 0;
    std::ifstream input_file(lexicon);

    if (!input_file.is_open()) {
        throw std::invalid_argument("Lexicon input file doesn't exist");
        exit(1);
    }

    //Initialize _lenghtMin
    std::string line;
    getline(input_file, line);
    _lengthMin = line.length();
    _nbWords ++;

    //Initialize _lenghtMax
    while(getline(input_file, line)){ //go to the end of the file
        _lengthMax = line.length();
        _nbWords ++;
    }

    // Initialize _dbwords
    _dbwords = (std::bitset<26> ***) malloc((_lengthMax+1) * sizeof(std::bitset<26> **));
    for (unsigned i = 0; i <= _lengthMax; i ++) {
        _dbwords[i] = (std::bitset<26> **) malloc(_nbWords * sizeof(std::bitset<26> *));
        for (unsigned j = 0; j < _nbWords; j ++) {
            _dbwords[i][j] = (std::bitset<26> *) malloc((_lengthMax + 1)* sizeof(std::bitset<26>));
        }
    }

    _maxWordsPerSize = (unsigned*)malloc((_lengthMax+1) * sizeof(unsigned));
    encodeLexiconToBits(lexicon);
    
    input_file.close();
}

/**
 * @brief Destroys the Lexicon object.
 * 
 */
Lexicon::~Lexicon(){
    for (unsigned i = 0; i <= _lengthMax; i ++) {
        for (unsigned j = 0; j < _nbWords; j ++) {
            free(_dbwords[i][j]);
        }
        free(_dbwords[i]);
    }
    free(_dbwords);
    free(_maxWordsPerSize);
}

/**
 * @brief Add the word to the encoded lexicon.
 *
 * @param lengthX Lenght of the word.
 * @param indexWord Index in lengthX table.
 * @param word Word to add from Lexicon to _dbwords[lengthX][indexWord][letter in {1,lengthX}].
 */
void Lexicon::encodeWordtoBits(unsigned lengthX, unsigned indexWord, std::string word) {
    for(int letter=0; letter < word.length(); letter++) {
        int indexChar = word[letter] - 'A';
        if (indexChar > 26 || indexChar < 0) {
            std::cerr << "Error letter :" << word[letter] << std::endl;
            exit(1);
        }
        _dbwords[lengthX][indexWord][letter].set(indexChar);
    }
}

/**
 * @brief Read the Lexicon file and convert its string words to bit array.
 *
 * @param lexicon Path to the database file.
 */
void Lexicon::encodeLexiconToBits(const char * lexicon) {

    std::ifstream input_file(lexicon);
    if (!input_file.is_open()) {
        std::cerr << "Error while reading input file : does not exist \n";
        exit(1);
    }

    unsigned indexInCurrentLength = 0;
    unsigned currentLength = 0;

    std::vector<std::string> lines;
    std::string line;
    while(getline(input_file, line)){ //while the file is not fully read
        lines.push_back(line); //push the new line at the end of the vector
    }

    for(const auto &word : lines){ //print words from vector
        if(word.length() != currentLength) {
            _maxWordsPerSize[currentLength] = indexInCurrentLength;
            currentLength = word.length();
            indexInCurrentLength = 0;
        }
        encodeWordtoBits(word.length(), indexInCurrentLength,  word);
        indexInCurrentLength ++;
    }
    input_file.close();
}

/**
 * @brief Prints all the words that are in the lexicon.
 */
void Lexicon::printDataBase() {
    for(unsigned curLength = _lengthMin; curLength <= _lengthMax; curLength ++) {
        for (unsigned word = 0; word < _nbWords; word ++) {
            bool flag = false;
            for(unsigned letter = 0 ; letter < curLength && _dbwords[curLength][word][letter].count() != 0 ; letter ++) {
                std::cout << _dbwords[curLength][word][letter] << "\n";
                flag = true;
            }
            if (flag) std::cout << "\n";
        }
    }
}

/**
 * @brief Convert a bit tab of lenght 26 to a char.
 *
 * @param letterToConvert The letter to convert.
 * 
 * @return Returns the converted char.
 */
char Lexicon::convertBitToChar(std::bitset<26> letterToConvert){
    char letter;
    for(unsigned i = 0 ; i < 26 ; i++){
        if(letterToConvert.test(i)){
            letter = 'A' + i;
        }
    }
    return letter;
}

/**
 * @brief Convert a bit word into string word.
 *
 * @param wordToConvert The word to convert.
 * @param wordlenght Its lenght.
 * 
 * @return Returns the word converted.
 */
char *Lexicon::convertBitToWord(std::bitset<26> *wordToConvert, unsigned wordlenght){
     char *word = (char *) malloc (sizeof(char) * wordlenght);
     for (unsigned pos = 0 ; pos < wordlenght ; pos ++){
         word[pos] = convertBitToChar(wordToConvert[pos]);
     }
     word[wordlenght] = '\0';
     return word;
}

/**
 * @brief Returns all the words of size @p lenght .
 *
 * @param lenght Lenght of the words we want to get.
 * 
 * @return Returns the words, encoded in binary.
 */
std::bitset<26> **Lexicon::getWordByLenght(int lenght){
    if(lenght > _lengthMax) throw LexiconException();
    return _dbwords[lenght];
}

/**
 * @brief Get the instance of the lexicon (Singleton).
 * 
 * @param filename Path to the database file.
 * 
 * @return Returns the Lexicon instance.
 */
Lexicon *Lexicon::getInstance(const char* filename)
{
    if(_singleton==nullptr){
        _singleton = new Lexicon(filename);
    }
    return _singleton;
}

/**
 * @brief Release the instance of the lexicon (Singleton).
 */
void Lexicon::releaseInstance(){	

	if(nullptr != _singleton)
	{
		delete _singleton;
		_singleton = nullptr;						
	}
}

/**
 * @brief Return the definition associated to the word @p word .
 * 
 * @param word The word to look for.
 * 
 * @return Returns the definition associated.
 */
std::string Lexicon::getAssociateClue(std::string word) {
    std::ifstream input_file("../../data/databases/word_with_def.txt");
    if (!input_file.is_open()) {
        throw std::invalid_argument("Lexicon input file doesn't exist");
        exit(1);
    }

    std::vector<std::string> lines;
    std::string line;
    while (getline(input_file, line)){
        lines.push_back(line);
        if(word.size() == line.size() - 1){
            bool equal = true;
            for(int i = 0 ; i < word.size() ; i++)
                if(word.at(i) != line.at(i))
                    equal = false;
            if(equal){
                getline(input_file,line);
                return line;
            }
        }
    }
    input_file.close();
    throw LexiconException();
}

/**
 * @return Returns the minimum length of the words in this lexicon.
 */
unsigned Lexicon::getLengthMin() {
    return _lengthMin;
}

/**
 * @return Returns the maximum length of the words in this lexicon.
 */
unsigned Lexicon::getLengthMax() {
    return _lengthMax;
}

/**
 * @return Returns the total number of words in this lexicon.
 */
unsigned Lexicon::getNbWords() {
    return _nbWords;
}

/**
 * @brief Get the total number of words for a given length.
 * 
 * @param size The length of the words.
 * 
 * @return Returns the total number of words.
 */
unsigned Lexicon::getMaxWordsForSize(unsigned size) {
    if(size > _lengthMax) throw LexiconException();
    return _maxWordsPerSize[size];
}
