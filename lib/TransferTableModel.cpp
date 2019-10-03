#include "TransferTableModel.h"

void TransferTableModel::clear() {
    beginResetModel();
    transfers_.clear();
    endResetModel();
}

void TransferTableModel::addTransfer( const TransferShareConstPtr& transfer ) {
    beginInsertRows( QModelIndex(), transfers_.size(), transfers_.size() );
    transfers_.push_back( transfer );
    endInsertRows();
}

TransferShareConstPtr TransferTableModel::transferShare( const QModelIndex& index ) const {
    return transfers_[index.row()];
}

void TransferTableModel::update( const QModelIndex& index ) {
    emit dataChanged( index, index );
}

void TransferTableModel::remove( const QModelIndex& index ) {
    beginResetModel();
    transfers_.removeAt( index.row() );
    endResetModel();
}

int TransferTableModel::rowCount( const QModelIndex& ) const {
    return transfers_.size();
}

int TransferTableModel::columnCount( const QModelIndex& ) const {
    return 3;
}

QVariant TransferTableModel::data( const QModelIndex& index, int role ) const {
    if( !index.isValid() ) {
        return QVariant();
    }

    if( role == Qt::DisplayRole ) {
        switch( index.column() ) {
            case 0:
                return transfers_[index.row()]->transfer()->date().toString( "yyyy-MM-dd" );
            case 1: {
                TransferConstPtr t = transfers_[index.row()]->transfer();
                if( t->privateDescription().isEmpty() ) {
                    return t->description();
                } else {
                    return t->description() + " < " + t->privateDescription() + " >";
                }
            }
            case 2:
                return formatCents( transfers_[index.row()]->cents() );
        }
    }

    if( role == Qt::EditRole ) {
        switch( index.column() ) {
            case 0:
                return transfers_[index.row()]->transfer()->date();
            case 1:
                return transfers_[index.row()]->transfer()->description();
            case 2:
                return transfers_[index.row()]->cents() / 100.0;
        }
    }

    if( role == Qt::TextAlignmentRole ) {
        switch( index.column() ) {
            case 0:
                return Qt::AlignLeft;
            case 1:
                return Qt::AlignLeft;
            case 2:
                return Qt::AlignRight;
        }
    }
    return QVariant();
}

QVariant TransferTableModel::headerData( int section, Qt::Orientation orientation, int role ) const {
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Horizontal ) {
        switch( section ) {
            case 0:
                return "Datum";
            case 1:
                return "Verwendungszweck";
            case 2:
                return "Betrag";
        }
    }

    if( orientation == Qt::Vertical ) {
        return section + 1;
    }

    return QVariant();
}
