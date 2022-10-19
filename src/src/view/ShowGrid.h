#ifndef SHOWGRID_H
#define SHOWGRID_H

#include <QAbstractTableModel>
#include "QDebug"
#include "QFont"
#include "QBrush"

#include "../controller/CrosswordController.h"

class CrosswordController;
/** @brief Player's grid that is displayed on the graphical interface.*/
class ShowGrid: public QAbstractTableModel
{

    Q_OBJECT

public:
    ShowGrid(QObject *parent,int row,int column, CrosswordController *g);
    ~ShowGrid();
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex & index) const ;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole); 
    void refreshGrid(std::vector<std::string> grid);

private:
    char ** _gridPlayer; ///< State of the player grid
    int _nbRow; ///< Number of rows in the definition table
    int _nbColumn; ///< Number of columns in the definition table
    CrosswordController* _controller; ///< Reference to the crossword controller

signals:
    void editCompleted(const QString &);

};

#endif // SHOWGRID_H
