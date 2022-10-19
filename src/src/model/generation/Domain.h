#ifndef DOMAIN_H
#define DOMAIN_H

#include "Variable.h"
#include <cstdlib>
#include <random>
#include <algorithm>

class Variable;
/** @brief Represents the domain of values for a Variable.*/
class Domain
{
    private:
        std::bitset<26> **_domain; ///< All words in the domain, encoded in binary
        Variable* _variable; ///< Points to the variable that has this domain
        unsigned _domainMaxSize; ///< The maximum size that the domain can take for a variable of this size
        unsigned _domainSize; ///< Current size of the domain
        std::vector<unsigned> _domainIndexList; ///< Lists all the word indexes that are in the domain

        void shuffleDomain();

    public:
        Domain(Variable* var);
        ~Domain();

        void initializeDomainValues();
        int findWord();
        void applyConstraint(char c, unsigned pos);
        void discardWordsWithLetterAtPos(char letter, unsigned pos);
        unsigned getDomainSize();
        std::bitset<26>* getWordInDomain(unsigned wordIndex); 

};
#endif
