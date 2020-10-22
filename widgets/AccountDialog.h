#pragma once

#include "Model.h"
#include "TransferTableModel.h"

#include <core/Account.h>

#include <QDialog>
#include <QSortFilterProxyModel>

namespace Ui {
class AccountDialog;
}

class AccountDialog : public QDialog {
    Q_OBJECT

public:
    explicit AccountDialog( AccountConstPtr account, Model& model, QWidget* parent = nullptr );
    ~AccountDialog() override;

    void accept() override;

private slots:
    void on_tableView_doubleClicked( const QModelIndex& index );
    void on_FilterLineEdit_textChanged( const QString& str );

private:
    Ui::AccountDialog* ui;
    AccountConstPtr account_;
    Model& model_;

    QSortFilterProxyModel sortModel_;
    TransferTableModel transferModel_;
};
