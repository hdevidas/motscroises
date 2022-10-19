#ifndef VARIABLE_H
#define VARIABLE_H

#include "Lexicon.h"
#include <map>
#include <stdlib.h>
#include "Domain.h"
#include "../utils.h"

class Domain;
/** @brief Class representing a variable in our CSP.*/
class Variable
{
    private:
        char* _word; ///< Value of the variable
        int _wordIndex = -1; ///< Word index in the Lexicon
        std::map<Variable*,int> _neighborhood; ///< For all variables that intersect with this variable we can retrieve the position of this intersection
        bool _horizontal; ///< True if the variable is horizontal in the grid, false otherwise
        unsigned _size; ///< Size of the variable
        Position _pos; ///< Position in the grid
        bool _inReset = false; ///< See if the variable is undergoing backtracking
        Domain* _domain; ///< Domain of the variable : all possibles words for the current variable (including all the constraints)


    public:
        Variable(bool horizontal, unsigned size, Position pos);
        ~Variable();

        int intersectWith(Variable* variable);
        void reset();
        void updateNeighbours(Variable* target = NULL);
        void conflict(Variable* var);

        void setWord(unsigned wordIndex);
        void addNeighboors(int pos, Variable* variable);
        void removeNeighboors(Variable* var);
        void noMoreReseting();
        Domain * getDomain();
        bool isHorizontal();
        unsigned getSize();
        Position getPos();
        std::map<Variable*,int> getNeighborhood();
        char* getWord();
        unsigned getWordIndex();
        bool isInReset();

        void printVariable();
};
#endif
