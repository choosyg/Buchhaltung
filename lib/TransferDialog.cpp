#include "TransferDialog.h"
#include "ui_TransferDialog.h"

TransferDialog::TransferDialog( TransferPtr transfer, QList< AccountPtr >& accounts, QWidget* parent )
    : QDialog( parent ), ui( new Ui::TransferDialog ), transfer_( transfer ), accounts_( accounts ) {
    ui->setupUi( this );
    setWindowFlags( windowFlags() & ~Qt::WindowContextHelpButtonHint );

    ui->calendarWidget->setSelectedDate( transfer->date() );
    ui->descriptionEdit->setText( transfer->description() );
    ui->amountSpinBox->setValue( transfer->cents() / 100.0 );

    ui->internalAccountBox->addItem( "Gemeinsam", 0 );
    size_t shareCount = 0;
    size_t externalShareId = 0;
    size_t internalShareId = 0;
    for( const auto& account : accounts ) {
        auto id = reinterpret_cast< size_t >( account.get() );
        if( account->type() == Account::Type::External ) {
            ui->externalAccountBox->addItem( account->name(), id );
            if( account->shares( transfer ) ) {
                externalShareId = id;
            }
        } else {
            ui->internalAccountBox->addItem( account->name(), id );
            if( account->shares( transfer ) ) {
                ++shareCount;
                internalShareId = id;
            }
        }
    }

    ui->externalAccountBox->setCurrentIndex( ui->externalAccountBox->findData( externalShareId ) );

    if( shareCount != 1 ) {
        ui->internalAccountBox->setCurrentIndex( ui->internalAccountBox->findData( 0 ) );
    } else {
        ui->internalAccountBox->setCurrentIndex( ui->internalAccountBox->findData( internalShareId ) );
    }
}

TransferDialog::~TransferDialog() {
    delete ui;
}

void TransferDialog::accept() {
    if( ui->internalAccountBox->currentIndex() < 0 || ui->externalAccountBox->currentIndex() < 0 ) {
        return;
    }

    transfer_->setDate( ui->calendarWidget->selectedDate() );
    transfer_->setDescription( ui->descriptionEdit->text() );
    transfer_->setCens( static_cast< int >( ui->amountSpinBox->value() * 100.0 ) );

    for( const auto& account : accounts_ ) {
        account->removeTransfer( transfer_ );
    }

    reinterpret_cast< Account* >( ui->externalAccountBox->currentData().toULongLong() )->addTransfer( transfer_ );

    if( ui->internalAccountBox->currentData().toULongLong() != 0 ) {
        reinterpret_cast< Account* >( ui->internalAccountBox->currentData().toULongLong() )->addTransfer( transfer_ );
    } else {
        size_t count = 0;
        for( const auto& account : accounts_ ) {
            if( account->type() == Account::Type::Internal ) {
                ++count;
            }
        }
        for( const auto& account : accounts_ ) {
            if( account->type() == Account::Type::Internal ) {
                account->addTransfer( transfer_, 1.0 / count );
            }
        }
    }

    QDialog::accept();
}
