#include "ShowGrid.h"
#include<QDebug>
#include <iostream>
#include <QMessageBox>

/**
 * @brief Build the definition player's grid.
 * 
 * @param parent Parent in the Qt hierarchy.
 * @param row Number of rows in the grid.
 * @param column Number of columns in the grid.
 * @param controller Reference to the crossword controller.
 */
ShowGrid::ShowGrid(QObject *parent,int row,int column,CrosswordController* controller)
    :QAbstractTableModel(parent)
{
    _nbColumn=column;
    _nbRow=row;
    _controller = controller;

    _gridPlayer = (char **) malloc (_nbRow * sizeof(char *));

    for (unsigned i = 0 ; i < _nbRow ; i++) {
        _gridPlayer[i]=(char *) malloc (_nbColumn * sizeof(char ));
    }

}

/**
 * @brief Count the rows number in the table.
 * 
 * @return Returns number of rows.
 */
int ShowGrid::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
   return _nbRow;
}

/**
 * @brief Count the columns number in the table.
 * 
 * @return Returns number of columns.
 */
int ShowGrid::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _nbColumn;
}

/**
 * @brief Determine what to display in the @p index box of the array, from the @p role .
 * 
 * @param index Qt index in the table.
 * @param role Role of the box.
 * 
 * @return Returns the Qt thing to display in the table.
 */
QVariant ShowGrid::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ) {
        return QVariant();
    }
    //add black cell
    int row=index.row();
    int col=index.column();
    char playerC=_gridPlayer[row][col];
        

        //change black case color
        if(playerC=='#' && Qt::BackgroundRole==role){
            QBrush blackBackground(Qt::black);
            return blackBackground;
                        
            
        }

        //change revealed case color
        if(playerC=='1' && Qt::BackgroundRole==role){

            QBrush redBackground(Qt::lightGray);
            return redBackground;
        }

        if (Qt::TextAlignmentRole==role) //change text alignment only for cell(1,1)
        {
            return  Qt::AlignVCenter+Qt::AlignHCenter;
            
        }
        if(Qt::ForegroundRole==role){
             QBrush blackText(Qt::black);
            return blackText;
        
        }        
        
        if(Qt::DisplayRole==role){
            if(playerC=='_' || playerC =='#')
                return QVariant();
            
           

            return QString(playerC);

        }
        return QVariant();
}

/**
 * @brief Triggered when the player change a letter in the grid (in the graphic interface).
 * 
 * @param index Qt index in the table.
 * @param value Qt value of the box.
 * @param role Role of the box.
 * 
 * @return Returns true if success.
 */
bool ShowGrid::setData(const QModelIndex & index, const QVariant & value, int role)
{
    int row=index.row();
    int column=index.column();
    
     if (role == Qt::EditRole)
    {
        char c=value.toString().toUpper().toStdString()[0];
        if(value.toString().size()==0){
                return true;
        }
        _gridPlayer[row][column] =c;
        _controller->letterChange(column,row,c);
    }
    return true;
}

/**
 * @brief Retrieves whether the box is selected or not.
 * 
 * @param index Qt index in the table.
 * 
 * @return Returns the Qt flags.
 */
Qt::ItemFlags ShowGrid::flags(const QModelIndex & index) const
{
   
    int row=index.row();
    int column=index.column();
    if(_gridPlayer[row][column]=='#' ||_gridPlayer[row][column]=='1')
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}

/**
 * @brief Refresh the display of the player's grid.
 * 
 * @param grid The new player's grid.
 */
void ShowGrid::refreshGrid(std::vector<std::string> grid)
{

    for(int i=0;i<_nbRow;i++){
        for(int j=0;j<_nbColumn;j++){
            _gridPlayer[i][j]=grid.at(i)[j];
        }

    }

}

/**
 * @brief Destroys the ShowGrid object
 * 
 */
ShowGrid::~ShowGrid(){
    for (unsigned i = 0 ; i < _nbRow ; i++) {
        free(_gridPlayer[i]);
    }
    free(_gridPlayer);
}


