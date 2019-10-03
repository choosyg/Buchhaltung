#include "InternalTransferDialog.h"
#include "ui_InternalTransferDialog.h"

#include <QCompleter>

InternalTransferDialog::InternalTransferDialog( TransferConstPtr transfer, Model& model, QWidget* parent )
    : QDialog( parent ), ui( new Ui::InternalTransferDialog ), model_( model ), transfer_( transfer ) {
    ui->setupUi( this );
    setWindowFlags( windowFlags() & ~Qt::WindowContextHelpButtonHint );

    ui->calendarWidget->setSelectedDate( transfer->date() );
    ui->descriptionEdit->setText( transfer->description() );
    ui->amountSpinBox->setValue( transfer->cents() / 100.0 );

    auto completer = new QCompleter( model.completions(), this );
    ui->descriptionEdit->setCompleter( completer );

    size_t sourceShareId = 0;
    size_t targetShareId = 0;
    for( const auto& account : model.accounts() ) {
        auto id = reinterpret_cast< size_t >( account.get() );
        if( test( account->flags(), Flags::External ) ) {
            ui->sourceAccountBox->addItem( account->name(), id );
            ui->targetAccountBox->addItem( account->name(), id );
            if( account->share( transfer ) < 0 ) {
                sourceShareId = id;
            }
            if( account->share( transfer ) > 0 ) {
                targetShareId = id;
            }
        }
    }

    if( sourceShareId != 0 ) {
        ui->sourceAccountBox->setCurrentIndex( ui->sourceAccountBox->findData( sourceShareId ) );
    } else {
        ui->sourceAccountBox->setCurrentIndex( 0 );
    }

    if( targetShareId != 0 ) {
        ui->targetAccountBox->setCurrentIndex( ui->targetAccountBox->findData( targetShareId ) );
    } else {
        ui->targetAccountBox->setCurrentIndex( 0 );
    }
}

InternalTransferDialog::~InternalTransferDialog() {
    delete ui;
}

void InternalTransferDialog::accept() {
    if( ui->sourceAccountBox->currentIndex() < 0 || ui->targetAccountBox->currentIndex() < 0 ) {
        return;
    }

    AccountConstPtr source = nullptr;
    AccountConstPtr target = nullptr;
    for( auto& account : model_.accounts() ) {
        auto id = reinterpret_cast< size_t >( account.get() );
        if( id == ui->sourceAccountBox->currentData().toULongLong() ) {
            source = account;
        }
        if( id == ui->targetAccountBox->currentData().toULongLong() ) {
            target = account;
        }
    }

    if( source == nullptr || target == nullptr ) {
        return;
    }

    model_.remove( transfer_ );
    auto transfer = std::make_shared< Transfer >( ui->calendarWidget->selectedDate(),
                                                  ui->descriptionEdit->text(),
                                                  round( ui->amountSpinBox->value() * 100.0 ) );

    model_.insertInternal( transfer, source, target );

    QDialog::accept();
}

void InternalTransferDialog::on_deleteButton_clicked() {
    model_.remove( transfer_ );
    QDialog::accept();
}
