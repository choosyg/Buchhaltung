#include "AccountOverviewTableModel.h"

#include <QColor>

void AccountOverviewTableModel::clear() {
    beginResetModel();
    overviews_.clear();
    endResetModel();
}

void AccountOverviewTableModel::addAccount( const AccountConstPtr& account ) {
    AccountOverview overview;
    overview.balance = account->balance();
    overview.balanceString = formatCents( overview.balance );
    overview.name = account->name();
    overview.account = account;

    beginInsertRows( QModelIndex(), overviews_.size(), overviews_.size() );
    overviews_.push_back( overview );
    endInsertRows();
}

AccountConstPtr AccountOverviewTableModel::account( const QModelIndex& index ) const {
    if( index.model() != this ) {
        throw std::runtime_error( "AccountOverviewTableModel::account called with wrong index" );
    }
    return overviews_[index.row()].account;
}

int AccountOverviewTableModel::rowCount( const QModelIndex& ) const {
    return overviews_.size();
}

int AccountOverviewTableModel::columnCount( const QModelIndex& ) const {
    return 2;
}

QVariant AccountOverviewTableModel::data( const QModelIndex& index, int role ) const {
    if( !index.isValid() ) {
        return QVariant();
    }

    if( role == Qt::DisplayRole ) {
        switch( index.column() ) {
            case 0:
                return overviews_[index.row()].name;
            case 1:
                return overviews_[index.row()].balanceString;
        }
    }

    if( role == Qt::EditRole ) {
        switch( index.column() ) {
            case 0:
                return overviews_[index.row()].name;
            case 1:
                return overviews_[index.row()].balance;
        }
    }

    if( role == Qt::ForegroundRole ) {
        if( overviews_[index.row()].balance < 0 ) {
            return QColor( Qt::red );
        }
        if( test( overviews_[index.row()].account->flags(), Flags::Closed ) ) {
            return QColor( Qt::gray );
        }
    }

    if( role == Qt::TextAlignmentRole ) {
        switch( index.column() ) {
            case 0:
                return static_cast< int >( Qt::AlignLeft | Qt::AlignVCenter );
            case 1:
                return static_cast< int >( Qt::AlignRight | Qt::AlignVCenter );
        }
    }
    return QVariant();
}

QVariant AccountOverviewTableModel::headerData( int section, Qt::Orientation orientation, int role ) const {
    if( role != Qt::DisplayRole ) {
        return QAbstractTableModel::headerData( section, orientation, role );
    }

    if( orientation == Qt::Horizontal ) {
        switch( section ) {
            case 0:
                return "Name";
            case 1:
                return "Kontostand";
        }
    }

    if( orientation == Qt::Vertical ) {
        return section + 1;
    }

    return QAbstractTableModel::headerData( section, orientation, role );
}
