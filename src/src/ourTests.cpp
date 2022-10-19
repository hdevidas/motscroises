#include <gtest/gtest.h>
#include "model/Crossword.h"

Lexicon* _lexicon = Lexicon::getInstance("../../data/databases/word_database.txt");

// --  Positive tests --

TEST(CrossedWordTest, Difficulty) {
    Crossword* game = new Crossword(EASY);
    ASSERT_EQ(game->getRemainingHelpPoints(), 100);
    ASSERT_EQ(game->getHeight(), 6);
    ASSERT_EQ(game->getWidth(), 6);
    delete game;

    game = new Crossword(NORMAL);
    ASSERT_EQ(game->getRemainingHelpPoints(), 80);
    ASSERT_EQ(game->getHeight(), 8);
    ASSERT_EQ(game->getWidth(), 8);
    delete game;

    game = new Crossword(HARD);
    ASSERT_EQ(game->getRemainingHelpPoints(), 50);
    ASSERT_EQ(game->getHeight(), 10);
    ASSERT_EQ(game->getWidth(), 10);
    delete game;
}

TEST(CrosswordTest, revealGrid) {
    Crossword* game = new Crossword(EASY);
    game->revealGrid();
    char ** curGrid = game->getCurrentGridTest();
    char ** finalGrid = game->getSolutionGridTest();

    for (unsigned line = 0; line < game->getHeight(); line ++)
        for (unsigned column = 0; column < game->getWidth(); column ++)
            ASSERT_EQ(curGrid[line][column], finalGrid[line][column]);
    delete game;
}

TEST(CrossedWordTest, Import) {
    // Import our test grid (generated from an Exported grid)
    Crossword* game = new Crossword("../../data/filesUTest/test.json");
    // Check grid parameters
    ASSERT_EQ(game->getRemainingHelpPoints(), 100);
    ASSERT_EQ(game->getHeight(), 6);
    ASSERT_EQ(game->getWidth(), 6);
    // Check current grid
    char compareCurrent[7][7] = {
                                    {"ABC###"},
                                    {"_##___"},
                                    {"______"},
                                    {"XTESTX"},
                                    {"___##_"},
                                    {"###MID"}
                                 };
    for (unsigned line = 0; line < 6; line ++)
        for (unsigned column = 0; column < 6; column ++)
            ASSERT_EQ(game->getLetter(line,column), compareCurrent[column][line]);
    // Check final grid
    char compareFinal[7][7] = {
                                    {"BEN###"},
                                    {"R##UAE"},
                                    {"AGENDA"},
                                    {"GOPHER"},
                                    {"SDI##E"},
                                    {"###MID"}
                                 };
    game->revealGrid();
    for (unsigned line = 0; line < 6; line ++)
        for (unsigned column = 0; column < 6; column ++)
            ASSERT_EQ(game->getLetter(line,column), compareFinal[column][line]);
    delete game;
}

TEST(CrossedWordTest, Export) {
    Crossword* game = new Crossword(EASY);
    game->exportGame("../../data/filesUTest/testExport.json");
    Crossword* gameImport = new Crossword("../../data/filesUTest/testExport.json");
    ASSERT_EQ(game->getHeight(), gameImport->getHeight());
    ASSERT_EQ(game->getWidth(), gameImport->getWidth());
    ASSERT_EQ(game->getRemainingHelpPoints(), gameImport->getRemainingHelpPoints());

    char ** curGrid = game->getCurrentGridTest();
    char ** curGridImport = gameImport->getCurrentGridTest();

    char ** finalGrid = game->getSolutionGridTest();
    char ** finalGridImport = gameImport->getSolutionGridTest();

    for (unsigned line = 0; line < game->getHeight(); line ++)
        for (unsigned column = 0; column < game->getWidth(); column ++) {
            ASSERT_EQ(curGrid[line][column], curGridImport[line][column]);
            ASSERT_EQ(finalGrid[line][column], finalGridImport[line][column]);
        }
    delete game;
    delete gameImport;
}

TEST(CrossedWordTest, checkHelpPointsCount_CheckLetter) {
    Crossword* game = new Crossword("../../data/filesUTest/test.json");
    int initHP = game->getRemainingHelpPoints();
    game->checkLetter(0,0);
    ASSERT_EQ(initHP - CHECK_LETTER, game->getRemainingHelpPoints());
    delete game;
}

TEST(CrossedWordTest, checkHelpPointsCount_CheckWord) {
    Crossword* game = new Crossword("../../data/filesUTest/test.json");
    int initHP = game->getRemainingHelpPoints();
    game->checkWord(1,0);
    ASSERT_EQ(initHP - CHECK_WORD, game->getRemainingHelpPoints());
    delete game;
}

TEST(CrossedWordTest, checkHelpPointsCount_RevealLetter) {
    Crossword* game = new Crossword("../../data/filesUTest/test.json");
    int initHP = game->getRemainingHelpPoints();
    game->revealLetter(0,0);
    ASSERT_EQ(initHP - REVEAL_LETTER, game->getRemainingHelpPoints());
    delete game;
}

TEST(CrossedWordTest, checkHelpPointsCount_RevealWord) {
    Crossword* game = new Crossword("../../data/filesUTest/test.json");
    int initHP = game->getRemainingHelpPoints();
    game->revealWord(1,0);
    ASSERT_EQ(initHP - REVEAL_WORD, game->getRemainingHelpPoints());
    delete game;
}

TEST(CrossedWordTest, checkLetter) {
    Crossword* game = new Crossword("../../data/filesUTest/test.json");
    ASSERT_EQ(1, game->checkLetter(5,5)); // Right letter
    ASSERT_EQ(0, game->checkLetter(0,0)); // Wrong letter
    for (unsigned i = 0; i < 98; i ++)
        game->checkLetter(5,5);
    ASSERT_EQ(-1, game->checkLetter(0,0)); // Not enough helpPoints
    delete game;
}

TEST(CrossedWordTest, checkWord) {
    Crossword* game = new Crossword("../../data/filesUTest/test.json");
    ASSERT_EQ(1, game->checkWord(6,1)); // Right word
    ASSERT_EQ(0, game->checkWord(1,1)); // Wrong word
    for (unsigned i = 0; i < 98; i ++)
        game->checkLetter(5,5);
    ASSERT_EQ(-1, game->checkWord(1,0)); // Not enough helpPoints
    delete game;
}

TEST(CrosswordTest, validateGrid) {
    Crossword* game = new Crossword("../../data/filesUTest/test.json");
    ASSERT_EQ(false, game->validateGrid());
    game->revealGrid();
    ASSERT_EQ(true, game->validateGrid());
    delete game;
}

TEST(CrosswordTest, writeLetter) { 
    Crossword* game = new Crossword("../../data/filesUTest/test.json");
    game->writeLetter(0,0,'A');
    char ** curGrid = game->getCurrentGridTest();
    ASSERT_EQ(curGrid[0][0], 'A');
}
    
TEST(CrosswordTest, resetGrid) {
    Crossword* game = new Crossword("../../data/filesUTest/test.json");
    game->resetGrid();
    char checkReset[7][7] = {
                                    {"___###"},
                                    {"_##___"},
                                    {"______"},
                                    {"______"},
                                    {"___##_"},
                                    {"###___"}
                                 };
    char ** curGridResetted = game->getCurrentGridTest();
    for (unsigned line = 0; line < game->getHeight(); line ++)
        for (unsigned column = 0; column < game->getWidth(); column ++)
            ASSERT_EQ(curGridResetted[line][column], checkReset[line][column]);
    delete game;
}

// --  Negative tests --

TEST(CrossedWordTest, ImportNegatifUnknownFIle) {
    try {
        new Crossword("../../data/filesUTest/fileDoesntExists.json");
        ASSERT_TRUE(false); // this should not be reached!
    } catch (...) {
        ASSERT_TRUE(true);
    }
}

TEST(CrossedWordTest, ImportNegatifCorruptedFile) {
    try {
        new Crossword("../../data/filesUTest/testNegatif.json");
        ASSERT_TRUE(false); // this should not be reached!
    } catch (...) {
        ASSERT_TRUE(true);
    }
}

TEST(CrosswordTest, writeBadLetter) { 
    Crossword* game = new Crossword("../../data/filesUTest/test.json");
    game->writeLetter(0,0,'3');
    char ** curGrid = game->getCurrentGridTest();
    ASSERT_EQ(curGrid[0][0], '_');
}