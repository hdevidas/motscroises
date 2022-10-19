#include "MainWindow.h"
#include <QMenuBar>
#include <QMessageBox>
#include <QtGui>
#include <QTextEdit>
#include <QIODevice>
#include <QHeaderView>
#include <QMovie>
#include <chrono>
#include <thread>
/**
 * @brief Builds the main window and links to triggers
 *
 * @param parent The object to host the grid and the differents actions to handle.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::actionQuit);
    connect(ui->actionAbout_the_Project, &QAction::triggered, this, &MainWindow::actionAbout);
    connect(ui->actionAbout_Qt, &QAction::triggered, this, &MainWindow::actionAboutQt);
    connect(ui->actionThe_Rules, &QAction::triggered, this, &MainWindow::help);
    connect(ui->easyButton, &QPushButton::released, this, &MainWindow::difficultyEasy);
    connect(ui->normalButton, &QPushButton::released, this, &MainWindow::difficultyNormal);
    connect(ui->hardButton, &QPushButton::released, this, &MainWindow::difficultyHard);
    connect(ui->revealGrid, &QPushButton::released, this, &MainWindow::handleButtonRevealGrid);
    connect(ui->validateGrid, &QPushButton::released, this, &MainWindow::handleButtonValidateGrid);
    connect(ui->checkLetter, &QPushButton::released,this, &MainWindow::handleButtonCheckLetter);
    connect(ui->checkWord, &QPushButton::released,this, &MainWindow::handleButtonCheckWord);
    connect(ui->revealword, &QPushButton::released, this, &MainWindow::handleButtonRevealWord);
    connect(ui->revealLetter, &QPushButton::released, this, &MainWindow::handleButtonRevealLetter);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::exportGrid);
    connect(ui->actionLoad, &QPushButton::released, this, &MainWindow::importGrid);
    connect(ui->Restart, &QPushButton::released, this, &MainWindow::handleButtonRestart);
    connect(ui->changeGrid, &QPushButton::released, this, &MainWindow::handleButtonChangeGrid);


    ui->gridTable->hide();
    ui->tableDef->hide();

    ui->changeGrid->hide();
    ui->revealGrid->hide();
    ui->validateGrid->hide();
    ui->checkLetter->hide();
    ui->revealLetter->hide();
    ui->checkWord->hide();
    ui->revealword->hide();
    ui->Restart->hide();
    ui->loadInformation->hide();
    ui->helpPoints->hide();

    ui->gridNumber->hide();

    const QString gridStyle = "QTableView { gridline-color: black}";
    const QString gridNumberStyle = "QTableView { gridline-color: transparent;background-color: transparent;}";
    const QString titleStyle = "QTextBrowser {border-color: transparent;background-color: transparent;}";
    ui->title->setStyleSheet(titleStyle);
    ui->gridTable->setStyleSheet(gridStyle);
    ui->gridNumber->setStyleSheet(gridNumberStyle);
    ui->helpPoints->setStyleSheet(titleStyle);
    ui->checkWord->setText("Check word (" + QString::number(CHECK_WORD) + ")");
    ui->checkLetter->setText("Check letter ("+ QString::number(CHECK_LETTER) + ")");
    ui->revealLetter->setText("Reveal letter (" + QString::number(REVEAL_LETTER) + ")");
    ui->revealword->setText("Reveal word (" + QString::number(REVEAL_WORD) + ")");

}

/**
 * @brief Destroys the MainWindow object.
 * 
 */
MainWindow::~MainWindow()
{
    delete _showGrid;
    delete _defTable;
    delete _controller;
    delete _defTable;
    delete ui;
}

/**
 * @brief Triggered when the player has pressed the "Quit" button: closes the window.
 */
void MainWindow::actionQuit()
{
    Lexicon::getInstance(NULL)->releaseInstance();
    QApplication::quit();
}

/**
 * @brief Triggered when the player has pressed the "About" button: display the credits.
 */
void MainWindow::actionAbout()
{
    QMessageBox::information(this, "About the Project", "This project has been developped by Clément Albanel, Emma Auzi, Yohan Bornes, Nathanael Alves, Hugo Devidas and Sylvain Coudougnan.");
}

/**
 * @brief Triggered when the player has pressed the "About Qt" button: display Qt informations.
 */
void MainWindow::actionAboutQt()
{
    QApplication::aboutQt();
}

/**
 * @brief Triggered when the player has pressed the "Help" button: show a tutorial to use our application.
 */
void MainWindow::help()
{
    QMessageBox::information(this, "The Rules", "Simply fill the field with words linked to their definition .... \n\nTo play the game, just click, in the starting menu, on the difficulty you want to play with.\n\nDuring the game, if you want a clue, click on the buttons below the grid. Please take note that you need to have enough help points to use them.\n\nWhen you finished the grid (when you think haved finished), click on Validate Grid.\n\nYou need more time or you have to go ? Don't worry, use the File menu to save your grid. Then, click on Load a save in the starting menu to restart the game (at the same point that you stopped).");
}

/**
 * @brief Triggered when the player has pressed the "Play -> Easy" button: launch the build of an easy crossword game.
 */
void MainWindow::difficultyEasy()
{
    constructGame(EASY);
}

/**
 * @brief Triggered when the player has pressed the "Play -> Normal" button: launch the build of an normal crossword game.
 */
void MainWindow::difficultyNormal()
{
    constructGame(NORMAL);
}

/**
 * @brief Triggered when the player has pressed the "Play -> Hard" button: launch the build of an hard crossword game.
 */
void MainWindow::difficultyHard()
{
    constructGame(HARD);
}

/**
 * @brief Launch the build of a crossword game with the given difficulty.
 *
 * @param diff The difficulty chosen.
 */
void MainWindow::constructGame(Level diff)
{
    ui->loadInformation->show();
    this->repaint();
    if (ui->gridTable->isVisible())
    {
        QMessageBox::information(this, "Grid", "There is already a generated grid !");
        return;
    }

    _controller = new CrosswordController(this);
    _controller->createGame(diff);
}

/**
 * @brief Initiates the display of a generated crossword.
 * 
 * @param heightOfGrid Height of the crossword grid.
 * @param widthOfGrid Width of the crossword grid.
 * @param grid Copy of the player grid.
 * @param words List of all useful data from our crossword words.
 * @param remainingHelpPoints Remaining help points of the player.
 */
void MainWindow::initializeDisplay(unsigned heightOfGrid,unsigned widthOfGrid, std::vector<std::string> grid, std::vector<Word> words, unsigned remainingHelpPoints){

    _showGrid = new ShowGrid(this, widthOfGrid, heightOfGrid, _controller); // creates the graphical grid
    _showNumber=new ShowNumber(this, widthOfGrid, heightOfGrid);
    char** numbers = (char **) malloc (heightOfGrid * sizeof(char *));
    for (unsigned i = 0 ; i < heightOfGrid ; i++) {
        numbers[i]=(char *) malloc (widthOfGrid * sizeof(char ));
        for(unsigned j = 0 ; j < widthOfGrid ; j++)
            numbers[i][j] = '_';
    }

    std::vector<Word> wordsH;
    std::vector<Word> wordsV;

    for (unsigned i=0; i<words.size(); i++){
        Word word =words.at(i);
        numbers[word.pos.y][word.pos.x] = word.number;
        if(word.horizontal){
            wordsH.push_back(word);
        }
        else{
            wordsV.push_back(word);
        }
    }
    _defTable=new DefTable(this,wordsH,wordsV);
   
    _showNumber->setNumbersInGrid(numbers);

    for (unsigned i = 0 ; i < heightOfGrid ; i++) {
        free(numbers[i]);
    }
    free(numbers);
    
    this->refreshGrid(grid);
    this->refreshHelpPoints(remainingHelpPoints);

    //for graphical display
    ui->gridTable->setModel(_showGrid);
    ui->gridNumber->setModel(_showNumber);
    ui->tableDef->setModel(_defTable);
    ui->gridTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->gridNumber->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->gridNumber->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->gridTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableDef->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableDef->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->gridTable->horizontalHeader()->hide();
    ui->gridTable->verticalHeader()->hide();
    ui->gridNumber->horizontalHeader()->hide();
    ui->gridNumber->verticalHeader()->hide();
    ui->tableDef->verticalHeader()->hide();

    ui->gridNumber->setAttribute( Qt::WA_TransparentForMouseEvents );

    ui->easyButton->hide();
    ui->hardButton->hide();
    ui->normalButton->hide();
    ui->actionLoad->hide();
    ui->title->hide();

    ui->loadInformation->hide();


    ui->gridTable->show();
    ui->tableDef->show();

    ui->helpPoints->show();
    ui->changeGrid->show();

    ui->revealGrid->show();
    ui->validateGrid->show();
    ui->checkLetter->show();
    ui->revealLetter->show();
    ui->checkWord->show();
    ui->revealword->show();
    ui->Restart->show();
    ui->gridNumber->show();

}

/**
 * @brief Refresh the display of the crossword.
 * 
 * @param grid Copy of the grid to be displayed. 
 */
void MainWindow::refreshGrid(std::vector<std::string> grid){
    _showGrid->refreshGrid(grid);
}

/**
 * @brief Refresh the display of the remaining help points of the player.
 * 
 * @param grid Remaining help points of the player. 
 */
void MainWindow::refreshHelpPoints(unsigned remainingHelpPoints){
    ui->helpPoints->setText("HELP POINTS : " + QString::number(remainingHelpPoints));
}

/**
 * @brief Trigger a popup on the interface.
 * 
 * @param text Text to display. 
 */
void MainWindow::popup(std::string text){
    QMessageBox::information(this, "Crossword", text.c_str());
}

/**
 * @brief Triggered when the player has pressed the "File -> Export" button: launch the exportation of the crossword.
 */
void MainWindow::exportGrid()
{
    if (!ui->gridTable->isVisible()) // if no grid is currently played
    {
        QMessageBox::information(this, "Grid", "You don't have any current grid to save ! ");
        return;
    }

    QString file_name = QFileDialog::getSaveFileName(this, "Open a file", "../../data/user_saves/my_grid.json");
    if(file_name==NULL){
        return;
    }
    std::string str_file_name = file_name.toStdString();

    _controller->exportGameAction(str_file_name);
}

/**
 * @brief Triggered when the player has pressed the "File -> Import" button: launch the importation of the crossword.
 */
void MainWindow::importGrid()
{
    if (ui->gridTable->isVisible())
    {
        QMessageBox::information(this, "Grid", "There is already a generated grid !");
        return;
    }

    QString file_name = QFileDialog::getOpenFileName(this, "Open a file", "../../data/user_saves");
    if(file_name==NULL){
        return;
    }
    std::string str_file_name = file_name.toStdString();

    _controller = new CrosswordController(this);
    _controller->importGame(str_file_name);
}

/**
 * @brief Triggered when the player has pressed the "Reveal Grid" button: launch the reveal of the crossword.
 */
void MainWindow::handleButtonRevealGrid()
{

    if (!ui->gridTable->isVisible())
    {
        QMessageBox::information(this, "Grid", "You don't have any current grid !");
        return;
    }
      QMessageBox::StandardButton reply = QMessageBox::question(this, "Are you sure ?", "Do you really want to reveal the grid ?",
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        _controller->revealGridAction();

    ui->gridTable->viewport()->update(); // update the graphic
    }
    
    
}

/**
 * @brief Triggered when the player has pressed the "Reveal Letter" button: launch the reveal of the concerned letter in the crossword.
 */
void MainWindow::handleButtonRevealLetter()
{
    if (!ui->gridTable->isVisible())
    {
        QMessageBox::information(this, "Grid", "You don't have any current grid !");
        return;
    }

    if(ui->gridTable->selectionModel()->selectedIndexes().empty()){
        QMessageBox::information(this, "Grid", "Please select a case to reveal !");
        return;
    }

    int row = ui->gridTable->selectionModel()->selectedIndexes()[0].row(); //the selected case in the grid (x)
    int col = ui->gridTable->selectionModel()->selectedIndexes()[0].column(); //the selected case in the grid (y)

    _controller->revealLetterAction(col,row);

    ui->gridTable->viewport()->update();
}

/**
 * @brief Triggered when the player has pressed the "Reveal Word" button: launch the reveal of the concerned word in the crossword.
 */
void MainWindow::handleButtonRevealWord()
{
    if (!ui->gridTable->isVisible())
    {
        QMessageBox::information(this, "Grid", "You don't have any current grid !");
        return;
    }
    if(ui->tableDef->selectionModel()->selectedIndexes().empty()){
        QMessageBox::information(this, "Grid", "Please select a word to reveal !");
        return;
    }
    QModelIndex index= ui->tableDef->selectionModel()->selectedIndexes()[0]; 
    
    QString data=index.data().toString();
    if(!data.isEmpty()){
        int num=data.split(" ")[0].toInt();
        if(index.column()){
           _controller->revealWordAction(num,false);
        }
        else{
            _controller->revealWordAction(num,true); 
        }
    }
    ui->gridTable->viewport()->update();
}

/**
 * @brief Triggered when the player has pressed the "Restart Grid" button: launch the restart of the crossword.
 */
void MainWindow::handleButtonRestart()
{
    if (!ui->gridTable->isVisible())
    {
        QMessageBox::information(this, "Grid", "You don't have any current grid !");
        return;
    }
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Are you sure ?", "Do you really want to reset the grid ?",
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
    _controller->restartAction();

    ui->gridTable->viewport()->update();
    }
}

/**
 * @brief Triggered when the player has pressed the "Validate Grid" button: launch the validation of the crossword.
 */
void MainWindow::handleButtonValidateGrid()
{
    if (!ui->gridTable->isVisible())
    {
        QMessageBox::information(this, "Grid", "You don't have any current grid !");
        return;
    }

    _controller->validateGridAction();

    ui->gridTable->viewport()->update();
}

/**
 * @brief Triggered when the player has pressed the "Check Letter" button: launch the checking of the concerned letter in the crossword.
 */
void MainWindow::handleButtonCheckLetter()
{
    if (!ui->gridTable->isVisible())
    {
        QMessageBox::information(this, "Grid", "You don't have any current grid !");
        return;
    }

    if(ui->gridTable->selectionModel()->selectedIndexes().empty()){
        QMessageBox::information(this, "Grid", "Please select a case to reveal !");
        return;
    }

    int row = ui->gridTable->selectionModel()->selectedIndexes()[0].row(); //the selected case in the grid (x)
    int col = ui->gridTable->selectionModel()->selectedIndexes()[0].column(); //the selected case in the grid (y)

    _controller->checkLetterAction(col,row);

    ui->gridTable->viewport()->update();
}

/**
 * @brief Triggered when the player has pressed the "Check Word" button: launch the checking of the concerned word in the crossword.
 */
void MainWindow::handleButtonCheckWord()
{
    if (!ui->gridTable->isVisible())
    {
        QMessageBox::information(this, "Grid", "You don't have any current grid !");
        return;
    }
    if(ui->tableDef->selectionModel()->selectedIndexes().empty()){
        QMessageBox::information(this, "Grid", "Please select a word to reveal !");
        return;
    }
    QModelIndex index= ui->tableDef->selectionModel()->selectedIndexes()[0]; 
    
    QString data=index.data().toString();
    if(!data.isEmpty()){
        int num=data.split(" ")[0].toInt();
        if(index.column()){
           _controller->checkWordAction(num,false);
        }
        else{
            _controller->checkWordAction(num,true); 
        }
    }
    ui->gridTable->viewport()->update();
}

/**
 * @brief Triggered when the player has pressed a key.
 *
 * @param event The key event that contains the key pressed.
 */
void MainWindow::keyPressEvent( QKeyEvent * event )
{
    if(ui->gridTable->isVisible() && !ui->gridTable->selectionModel()->selectedIndexes().empty()){
        if(event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete){
            int row = ui->gridTable->selectionModel()->selectedIndexes()[0].row(); //the selected case in the grid (x)
            int col = ui->gridTable->selectionModel()->selectedIndexes()[0].column(); //the selected case in the grid (y)
            _controller->letterChange(col,row,'_');
            ui->gridTable->viewport()->update();
        }
    }

}

/**
 * @brief Triggered when the player has pressed the "Change grid" button: brings the player to the menu to choose a new grid.
 */
void MainWindow::handleButtonChangeGrid(){

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Are you sure ?", "Do you really want to generate a new grid ?",
                                QMessageBox::Yes|QMessageBox::No | QMessageBox::Save);
    if(reply == QMessageBox::Save){
        exportGrid();
    }
    else if(reply==QMessageBox::No){
        return;
    }

    //hide the game
    ui->gridTable->hide();
    ui->tableDef->hide();
    ui->revealGrid->hide();
    ui->validateGrid->hide();
    ui->checkLetter->hide();
    ui->revealLetter->hide();
    ui->checkWord->hide();
    ui->revealword->hide();
    ui->Restart->hide();
    ui->loadInformation->hide();
    ui->helpPoints->hide();
    ui->changeGrid->hide();
    ui->gridNumber->hide();

    delete _showGrid;
    delete _defTable;
    delete _showNumber;
    delete _controller;
    _showGrid = nullptr;
    _defTable = nullptr;
    _showNumber=nullptr;
    _controller = nullptr;

    //show menu
    ui->easyButton->show();
    ui->hardButton->show();
    ui->normalButton->show();
    ui->actionLoad->show();
    ui->title->show();
}