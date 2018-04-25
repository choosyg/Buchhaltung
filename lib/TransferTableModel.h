#pragma once

#include "TransferShare.h"

#include <QAbstractTableModel>

class TransferTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    void clear();
    void addTransfer( const TransferSharePtr& transfer );
    TransferSharePtr transfer( const QModelIndex& index ) const;
    void update( const QModelIndex& index );
    void remove( const QModelIndex& index );

    int rowCount( const QModelIndex& parent ) const override;
    int columnCount( const QModelIndex& parent ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    bool setData( const QModelIndex& index, const QVariant& value, int role ) override;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
    Qt::ItemFlags flags( const QModelIndex& index ) const override;

private:
    QList< TransferSharePtr > transfers_;
};
