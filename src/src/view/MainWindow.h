#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QPushButton"
#include "QTableView"
#include "ui_mainwindow.h"
#include "QPushButton"
#include <QString>
#include <QFileDialog>
#include <QHeaderView>
#include <QFile>
#include <QTableView>
#include "ShowGrid.h"
#include "ShowNumber.h"
#include "DefTable.h"
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CrosswordController;
class ShowGrid;
/** @brief Class of the main window of the graphic interface.*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initializeDisplay(unsigned heightOfGrid,unsigned widthOfGrid, std::vector<std::string> grid, std::vector<Word> words, unsigned remainingHelpPoints);
    void refreshGrid(std::vector<std::string> grid);
    void refreshHelpPoints(unsigned remainingHelpPoints);
    void popup(std::string text);
    
private slots:
  void handleButtonRevealGrid();
  void actionQuit();
  void actionAbout();
  void help();
  void difficultyEasy();
  void difficultyNormal();
  void difficultyHard();
  void constructGame(Level diff);
  void handleButtonValidateGrid();
  void handleButtonRevealLetter();
  void exportGrid();
  void importGrid();
  void actionAboutQt();
  void handleButtonRevealWord();
  void handleButtonRestart();
  void handleButtonCheckLetter();
  void handleButtonCheckWord();
  void keyPressEvent( QKeyEvent * event );
  void handleButtonChangeGrid();

private:
    Ui::MainWindow *ui; ///< Reference to the user interface.
    ShowGrid *_showGrid  = nullptr; ///< The player grid
    ShowNumber *_showNumber  = nullptr; ///< Words numbers that are displayed in the player grid
    CrosswordController* _controller = nullptr; ///< Controller to interact with the game state
    DefTable *_defTable=nullptr; ///< Table of the associated definitions of words

};
#endif // MAINWINDOW_H
