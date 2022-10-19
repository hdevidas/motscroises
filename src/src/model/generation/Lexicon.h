#ifndef LEXICON_H
#define LEXICON_H

#include <bitset>
#include <iostream>
#include <cstring>
#include <fstream>
#include <exception>
#include <vector>
#include <string> 
// #include "QDebug"

/** @brief Exception thrown when trying to access to a word that is not in the lexicon.*/
struct LexiconException : public std::exception {
    /** @brief Exception message.*/
   const char * what () const throw () {
      return "LexiconException: tried to get words that aren't in the lexicon (verify the database file is not corrupted).";
   }
};

/** @brief The application's lexicon, contains words encoded in binary.*/
class Lexicon
{
    private:
        static Lexicon* _singleton; ///< Lexicon singleton
        unsigned _lengthMin; ///< The minimum length of a word in this lexicon
        unsigned _lengthMax; ///< The maximum length of a word in this lexicon
        unsigned _nbWords; ///< Total number of words in this lexicon
        std::bitset<26> ***_dbwords; ///< All the words encoded in binary [<length>][<word>][<letter>]
        unsigned* _maxWordsPerSize; ///< Store all the total number of words for each length in this lexicon
        void encodeLexiconToBits(const char * lexicon);
        void encodeWordtoBits(unsigned word, unsigned index, std::string w);
        Lexicon(const char * filename);
        ~Lexicon();
    public:
        Lexicon(Lexicon &other) = delete;

        void operator=(const Lexicon &) = delete;

        static Lexicon* getInstance(const char * filename);
        static void releaseInstance();

        static char convertBitToChar(std::bitset<26> letterToConvert);
        static char* convertBitToWord(std::bitset<26> *wordToConvert, unsigned wordlenght);
        std::bitset<26>** getWordByLenght(int lenght);
        unsigned getMaxWordsForSize(unsigned size);

        void printDataBase();
        unsigned getLengthMin();
        unsigned getLengthMax();
        unsigned getNbWords();
        static std::string getAssociateClue(std::string word);

};
#endif
