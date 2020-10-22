#include "AccountDialog.h"
#include "ui_AccountDialog.h"

#include "InternalTransferDialog.h"
#include "TransferDialog.h"

AccountDialog::AccountDialog( AccountConstPtr account, Model& model, QWidget* parent )
    : QDialog( parent ), ui( new Ui::AccountDialog ), model_( model ), account_( account ) {
    ui->setupUi( this );
    setWindowFlags( windowFlags() & ~Qt::WindowContextHelpButtonHint );

    ui->nameLineEdit->setText( account->name() );
    if( test( account->flags(), Flags::Internal ) ) {
        ui->internalRadioButton->setChecked( true );
    } else {
        ui->externalRadioButton->setChecked( true );
    }
    ui->adressEdit->appendHtml( account->adress() );

    ui->balanceLabel->setText( formatCents( account->balance() ) );

    for( const auto& transfer : account->transferShares() ) {
        transferModel_.addTransfer( transfer );
    }

    sortModel_.setFilterRole( Qt::DisplayRole );
    sortModel_.setFilterKeyColumn( 1 );
    sortModel_.setFilterCaseSensitivity( Qt::CaseInsensitive );
    sortModel_.setSourceModel( &transferModel_ );

    ui->tableView->setModel( &sortModel_ );
    ui->tableView->resizeColumnsToContents();
    ui->tableView->sortByColumn( 0 );
}

AccountDialog::~AccountDialog() {
    delete ui;
}

void AccountDialog::accept() {
}

void AccountDialog::on_tableView_doubleClicked( const QModelIndex& index ) {
    auto si = sortModel_.mapToSource( index );
    auto transfer = transferModel_.transferShare( si )->transfer();

    auto accounts = model_.accounts();
    size_t shareCount = 0;
    double firstShare = 0;
    double secondShare = 0;
    for( const auto& account : accounts ) {
        auto share = account->share( transfer );
        if( share != 0 ) {
            ++shareCount;
            if( shareCount == 1 ) {
                firstShare = share;
            }
            if( shareCount == 2 ) {
                secondShare = share;
            }
        }
    }

    if( shareCount == 2 && firstShare == -secondShare ) {
        InternalTransferDialog( transfer, model_, this ).exec();
    } else {
        TransferDialog( transfer, model_, this ).exec();
    }
    transferModel_.clear();
    for( const auto& transfer : account_->transferShares() ) {
        transferModel_.addTransfer( transfer );
    }
}

void AccountDialog::on_FilterLineEdit_textChanged( const QString& str ) {
    sortModel_.setFilterFixedString( str );
}
