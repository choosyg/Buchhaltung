#pragma once

#include <core/Account.h>

#include <QAbstractTableModel>

class AccountOverviewTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    void clear();
    void addAccount( const AccountConstPtr& account );
    AccountConstPtr account( const QModelIndex& index ) const;

    int rowCount( const QModelIndex& parent ) const override;
    int columnCount( const QModelIndex& parent ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;

private:
    class AccountOverview {
    public:
        QString balanceString;
        QString name;
        int balance;
        AccountConstPtr account;
    };
    QList< AccountOverview > overviews_;
};
