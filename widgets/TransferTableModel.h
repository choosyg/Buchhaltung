#pragma once

#include <core/TransferShare.h>

#include <QAbstractTableModel>

class TransferTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    void clear();
    void addTransfer( const TransferShareConstPtr& transferShare );
    TransferShareConstPtr transferShare( const QModelIndex& index ) const;
    void update( const QModelIndex& index );
    void remove( const QModelIndex& index );

    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;

private:
    QList< TransferShareConstPtr > transfers_;
};
