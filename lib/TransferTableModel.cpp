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

int TransferTableModel::rowCount( const QModelIndex& parent ) const {
    return transfers_.size();
}

int TransferTableModel::columnCount( const QModelIndex& parent ) const {
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
                break;
            case 1:
                return transfers_[index.row()]->transfer()->description();
                break;
            case 2:
                return formatCents( transfers_[index.row()]->cents() );
                break;
        }
    }

    if( role == Qt::EditRole ) {
        switch( index.column() ) {
            case 0:
                return transfers_[index.row()]->transfer()->date();
                break;
            case 1:
                return transfers_[index.row()]->transfer()->description();
                break;
            case 2:
                return transfers_[index.row()]->cents() / 100.0;
                break;
        }
    }

    if( role == Qt::TextAlignmentRole ) {
        switch( index.column() ) {
            case 0:
                return Qt::AlignLeft | Qt::AlignVCenter;
                break;
            case 1:
                return Qt::AlignLeft | Qt::AlignVCenter;
                break;
            case 2:
                return Qt::AlignRight | Qt::AlignVCenter;
                break;
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
                break;
            case 1:
                return "Verwendungszweck";
                break;
            case 2:
                return "Betrag";
                break;
        }
    }

    if( orientation == Qt::Vertical ) {
        return section + 1;
    }

    return QVariant();
}
