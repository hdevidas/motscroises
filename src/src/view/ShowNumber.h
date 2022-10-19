#ifndef SHOWNUMBER_H
#define SHOWNUMBER_H

#include <QAbstractTableModel>
#include "QDebug"
#include "QFont"
#include "QBrush"
#include <stdlib.h>
#include <cstring>

/** @brief Class that display numbers associated to the words in the grid.*/
class ShowNumber: public QAbstractTableModel
{

    Q_OBJECT

public:
    ShowNumber(QObject *parent,int row,int column);
    ~ShowNumber();
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex & index) const ;
    void refreshGrid(std::vector<std::string> grid);
    void setNumbersInGrid(char** numbers);

private:
    int _nbRow; ///< Number of rows in the definition table
    int _nbColumn; ///< Number of columns in the definition table
    char ** _gridNumber; ///< Grid with numbers

signals:
    void editCompleted(const QString &);

};

#endif // SHOWGRID_H
