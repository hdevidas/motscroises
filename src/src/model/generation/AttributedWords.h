#ifndef ATTRIBUTEDWORDS_H
#define ATTRIBUTEDWORDS_H

#include "Lexicon.h"
#include <map>

/** @brief Class that makes an inventory of the words that are already placed in the grid.*/
class AttributedWords
{
    private:
        std::map<unsigned,bool*> forbiddenWords; ///< At <word_length> we get the boolean array that lists all words of <word_length>
    public:
        AttributedWords();
        ~AttributedWords();
        void addAttributedWord(unsigned size, unsigned index);
        void removeAttributedWord(unsigned size, unsigned index);
        bool isAttributed(unsigned size, unsigned index);
};

#endif