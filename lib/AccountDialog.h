#pragma once

#include "Account.h"
#include "Model.h"
#include "TransferTableModel.h"

#include <QDialog>
#include <QSortFilterProxyModel>

namespace Ui {
class AccountDialog;
}

class AccountDialog : public QDialog {
    Q_OBJECT

public:
    explicit AccountDialog( AccountConstPtr account, Model& model, QWidget* parent = 0 );
    ~AccountDialog();

    void accept() override;

private slots:
    void on_tableView_doubleClicked( const QModelIndex& index );

private:
    Ui::AccountDialog* ui;
    AccountConstPtr account_;
    Model& model_;

    QSortFilterProxyModel sortModel_;
    TransferTableModel transferModel_;
};
