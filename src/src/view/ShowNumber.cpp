#include "ShowNumber.h"
#include<QDebug>
#include <iostream>
#include <QMessageBox>

/**
 * @brief Build the show number class.
 * 
 * @param parent Parent in the Qt hierarchy.
 * @param row Number of rows in the grid.
 * @param column Number of columns in the grid.
 */
ShowNumber::ShowNumber(QObject *parent,int row,int column)
    :QAbstractTableModel(parent)
{
    _nbColumn=column;
    _nbRow=row;
        _gridNumber = (char **) malloc (_nbRow * sizeof(char *));
    for (unsigned i = 0 ; i < _nbRow ; i++) {
        _gridNumber[i]=(char *) malloc (_nbColumn * sizeof(char ));
    }
    
}

/**
 * @brief Count the rows number in the table.
 * 
 * @return Returns number of rows.
 */
int  ShowNumber::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
   return _nbRow;
}

/**
 * @brief Count the columns number in the table.
 * 
 * @return Returns number of columns.
 */
int ShowNumber::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _nbColumn;
}

/**
 * @brief Determine what to display in the @p index box of the array, from the @p role.
 * 
 * @param index Qt index in the table.
 * @param role Role of the box.
 * 
 * @return Returns the Qt thing to display in the table.
 */
QVariant ShowNumber::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ) {
        return QVariant();
    }

    int row=index.row();
    int col=index.column();

        if (Qt::TextAlignmentRole==role) //change text alignment only for cell(1,1)
        {
            return Qt::AlignLeft;
        }  
        
        if(Qt::DisplayRole==role)
            if(_gridNumber[row][col] != '_')
                return _gridNumber[row][col];
        if(Qt::ForegroundRole==role){
             QBrush blackText(Qt::black);
            return blackText;
        
        }
        if( Qt::BackgroundRole==role){
            QBrush transparent(Qt::transparent);
            return transparent;
                        
            
        }  
        return QVariant();
}

/**
 * @brief Set the numbers in the grid.
 * 
 * @param number Grid with numbers.
 */
void ShowNumber::setNumbersInGrid(char** numbers){
    for(int i=0;i<_nbRow;i++){
        std::memcpy(_gridNumber[i],numbers[i], _nbColumn * sizeof(char));
    }
}

/**
 * @brief Retrieves whether the box is selected or not.
 * 
 * @param index Qt index in the table.
 * 
 * @return Returns the Qt flags.
 */
Qt::ItemFlags ShowNumber::flags(const QModelIndex & index) const
{
        return Qt::NoItemFlags;
}

/**
 * @brief Destroys the ShowNumber object.
 * 
 */
ShowNumber::~ShowNumber(){
    for (unsigned i = 0 ; i < _nbRow ; i++) {
        free(_gridNumber[i]);
    }
    free(_gridNumber);
}