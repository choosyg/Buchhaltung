#include "TransferDialog.h"
#include "ui_TransferDialog.h"

TransferDialog::TransferDialog( TransferConstPtr transfer, Model& model, QWidget* parent )
    : QDialog( parent ), ui( new Ui::TransferDialog ), model_( model ), transfer_( transfer ) {
    ui->setupUi( this );
    setWindowFlags( windowFlags() & ~Qt::WindowContextHelpButtonHint );

    ui->calendarWidget->setSelectedDate( transfer->date() );
    ui->descriptionEdit->setText( transfer->description() );
    ui->amountSpinBox->setValue( transfer->cents() / 100.0 );

    ui->internalAccountBox->addItem( "Gemeinsam", 0 );
    size_t shareCount = 0;
    size_t externalShareId = 0;
    size_t internalShareId = 0;
    for( const auto& account : model.accounts() ) {
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

    auto transfer = std::make_shared< Transfer >( ui->calendarWidget->selectedDate(),
                                                  ui->descriptionEdit->text(),
                                                  ui->amountSpinBox->value() * 100.0 );

    AccountConstPtr external = nullptr;
    AccountConstPtr internal = nullptr;
    for( auto& account : model_.accounts() ) {
        auto id = reinterpret_cast< size_t >( account.get() );
        if( id == ui->externalAccountBox->currentData().toULongLong() ) {
            external = account;
        }
        if( id == ui->internalAccountBox->currentData().toULongLong() ) {
            internal = account;
        }
    }

    model_.remove( transfer_ );
    model_.insert( transfer, external, internal );

    QDialog::accept();
}
