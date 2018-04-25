#pragma once

#include "Account.h"
#include "TransferTableModel.h"

#include <QDialog>
#include <QSortFilterProxyModel>

namespace Ui {
class AccountDialog;
}

class AccountDialog : public QDialog {
    Q_OBJECT

public:
    explicit AccountDialog( AccountPtr account, QList< AccountPtr >& accounts, QWidget* parent = 0 );
    ~AccountDialog();

    void accept() override;

private slots:
    void on_tableView_doubleClicked( const QModelIndex& index );

private:
    Ui::AccountDialog* ui;
    AccountPtr account_;
    QList< AccountPtr >& accounts_;

    QSortFilterProxyModel sortModel_;
    TransferTableModel transferModel_;
};
