#ifndef DEFTABLE_H
#define DEFTABLE_H

#include <QAbstractTableModel>
#include "QDebug"
#include "QFont"
#include "QBrush"
#include <stdlib.h>
#include <cstring>
#include "../model/utils.h"

/** @brief Table in the interface that displays the definitions associated with the words of the crossword.*/
class DefTable: public QAbstractTableModel
{

    Q_OBJECT

public:
    DefTable(QObject *parent,std::vector<Word> wordsHori,std::vector<Word> wordsVerti);
    ~DefTable();
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex & index) const ;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    int _nbRow; ///< Number of rows in the definition table
    int _nbColumn; ///< Number of columns in the definition table
    std::vector<Word> _wordsH; ///< List of horizontal words
    std::vector<Word> _wordsV; ///< List of vertical words


signals:
    void editCompleted(const QString &);

};

#endif // SHOWGRID_H
