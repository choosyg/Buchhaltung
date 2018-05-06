#include "AccountDialog.h"
#include "ui_AccountDialog.h"

#include "TransferDialog.h"

AccountDialog::AccountDialog( AccountConstPtr account, Model& model, QWidget* parent )
    : QDialog( parent ), ui( new Ui::AccountDialog ), model_( model ), account_( account ) {
    ui->setupUi( this );
    setWindowFlags( windowFlags() & ~Qt::WindowContextHelpButtonHint );

    ui->nameLineEdit->setText( account->name() );
    if( account->type() == Account::Type::Internal ) {
        ui->internalRadioButton->setChecked( true );
    } else {
        ui->externalRadioButton->setChecked( true );
    }

    ui->balanceLabel->setText( formatCents( account->balance() ) );

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
    TransferDialog( transferModel_.transferShare( si )->transfer(), model_, this ).exec();
    if( account_->shares( transferModel_.transferShare( si )->transfer() ) ) {
        transferModel_.update( si );
    } else {
        transferModel_.remove( si );
    }
}
