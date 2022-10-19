#include "Pattern.h"

/**
 * @brief Construct an object representing the pattern of the grid by convention '_' is empty block and '#' is unusable block.
 * 
 * @param pattern Path to the pattern txt file.
 */
Pattern::Pattern(std::string pattern) {
    std::fstream readGrid;
    readGrid.open(pattern,std::ios::in);
    if (!readGrid.is_open()) {
        throw std::invalid_argument("Pattern input file doesn't exist");
    }
    std::string tmp;

    getline(readGrid, tmp);
    unsigned lineNumber = std::stoi(tmp);
    if(lineNumber < MIN_PATTERN_SIZE || lineNumber > MAX_PATTERN_SIZE) throw PatternException();
    this->_width = lineNumber;

    getline(readGrid, tmp);
    lineNumber = std::stoi(tmp);
    if(lineNumber < MIN_PATTERN_SIZE || lineNumber > MAX_PATTERN_SIZE) throw PatternException();
    this->_height = lineNumber;

    _grid = (char **)malloc(_height * sizeof(char *));
    for (unsigned i = 0; i < _height; i++) {
        _grid[i] = (char *)malloc(_width * sizeof(char));
    }

    unsigned heightIndex = 0;
    while(getline(readGrid, tmp)) {
        if (heightIndex >= _height) throw PatternException();
        // Check that each letter isIn '_'|| '#' and fulfill the grid
        for (unsigned letterLine = 0; letterLine < _width; letterLine ++) {
            if(letterLine >= tmp.length()) throw PatternException();
            int letterASCII = tmp[letterLine] - 'A';

            if (!(  tmp[letterLine] == '_' 
                ||  tmp[letterLine] == '#' 
                ) ){
                throw PatternException();
            }
            _grid[heightIndex][letterLine] = tmp[letterLine];
        }

        heightIndex ++;
    }

    if(heightIndex < _height) throw PatternException();

    readGrid.close();
}

/**
 * @brief Destroys the Pattern object.
 * 
 */
Pattern::~Pattern() {
    for(int i=0; i<_height; i++)
        free(_grid[i]);

    free(_grid);
}

/**
 * @return Returns width of the pattern. 
 */
unsigned Pattern::getWidth() {
    return _width;
}

/**
 * @return Returns height of the pattern. 
 */
unsigned Pattern::getHeight(){
    return _height;
}

/**
 * @return Returns the pattern grid. 
 */
char** Pattern::getGrid(){
    return _grid;
}
