#include "AccountDialog.h"
#include "ui_AccountDialog.h"

#include "TransferDialog.h"

AccountDialog::AccountDialog( AccountPtr account, QList< AccountPtr >& accounts, QWidget* parent )
    : QDialog( parent ), ui( new Ui::AccountDialog ), account_( account ), accounts_( accounts ) {
    ui->setupUi( this );

    ui->nameLineEdit->setText( account->name() );
    if( account->type() == Account::Type::Internal ) {
        ui->internalRadioButton->setChecked( true );
    } else {
        ui->externalRadioButton->setChecked( true );
    }

    ui->balanceLabel->setText( account->balance().toString() );

    for( const auto& transfer : account->transferShares() ) {
        transferModel_.addTransfer( transfer );
    }

    sortModel_.setSourceModel( &transferModel_ );
    ui->tableView->setModel( &sortModel_ );
    ui->tableView->resizeColumnsToContents();
}

AccountDialog::~AccountDialog() {
    delete ui;
}

void AccountDialog::accept() {
}

void AccountDialog::on_tableView_doubleClicked( const QModelIndex& index ) {
    auto si = sortModel_.mapToSource( index );
    TransferDialog( transferModel_.transfer( si )->transfer(), accounts_, this ).exec();
    if( account_->shares( transferModel_.transfer( si )->transfer() ) ) {
        transferModel_.update( si );
    } else {
        transferModel_.remove( si );
    }
}
