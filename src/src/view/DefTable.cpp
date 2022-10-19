#include "DefTable.h"

/**
 * @brief Build the definition table.
 * 
 * @param parent Parent in the Qt hierarchy.
 * @param wordsHori List of horizontal words.
 * @param wordsVerti List of vertical words.
 */
DefTable::DefTable(QObject *parent,std::vector<Word> wordsHori,std::vector<Word> wordsVerti)
    :QAbstractTableModel(parent)
{
    _nbColumn=2;
    if(wordsHori.size()<wordsVerti.size()){
        _nbRow=wordsVerti.size();
    }
    else{
        _nbRow=wordsHori.size();
    }
    _wordsH=wordsHori;
    _wordsV=wordsVerti;

}
/**
 * @brief Count the rows number in the table.
 * 
 * @return Returns number of rows.
 */
int  DefTable::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
   return _nbRow;
}

/**
 * @brief Count the columns number in the table.
 * 
 * @return Returns number of columns.
 */
int DefTable::columnCount(const QModelIndex &parent) const
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
QVariant DefTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ) {
        return QVariant();
    }
    //add black cell
    int row=index.row();
    int col=index.column();

        
        
        if(Qt::DisplayRole==role){
            if(col){
                if(row>=_wordsV.size()){
                    return QVariant();
                }
                Word w=_wordsV[row];
                return QString::number(w.number)+" "+QString::fromUtf8(w.associateClue.c_str());
            }
            else{
                if(row>=_wordsH.size()){
                    return QVariant();
                }
                Word w=_wordsH[row];
                return QString::number(w.number)+" "+QString::fromUtf8(w.associateClue.c_str());
            }
        }
        if(Qt::ForegroundRole==role){
             QBrush blackText(Qt::black);
            return blackText;
        
        }
       
        return QVariant();
}

/**
 * @brief Retrieves whether the box is selected or not.
 * 
 * @param index Qt index in the table.
 * 
 * @return Returns the Qt flags.
 */
Qt::ItemFlags DefTable::flags(const QModelIndex & index) const
{
   
    int row=index.row();
    int column=index.column();
    
    return Qt::ItemIsSelectable |  Qt::ItemIsEnabled ;
}

/**
 * @brief Display the header of the table.
 * 
 * @param section Identify the column horizontal or vertical.
 * @param orientation Qt orientation of table.
 * @param role Role of the box.
 * 
 * @return Returns the Qt thing to display.
 */
QVariant DefTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("Horizontal");
            case 1:
                return QString("Vertical");
            }
        }
    }
    return QVariant();
}

/**
 * @brief Destroys the DefTable object
 * 
 */
DefTable::~DefTable(){
    _wordsH.clear();
    _wordsV.clear();
}