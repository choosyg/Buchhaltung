#pragma once

#include "Account.h"

#include <QAbstractTableModel>

class AccountOverviewTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    void clear();
    void addAccount( const AccountPtr& account );
    AccountPtr account( const QModelIndex& index ) const;

    int rowCount( const QModelIndex& parent ) const override;
    int columnCount( const QModelIndex& parent ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;

private:
    class AccountOverview {
    public:
        QString balance;
        QString name;
        AccountPtr account;
    };
    QList< AccountOverview > overviews_;
};
