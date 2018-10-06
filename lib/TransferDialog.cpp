#include "TransferDialog.h"
#include "ui_TransferDialog.h"

#include <QCompleter>

TransferDialog::TransferDialog( TransferConstPtr transfer, Model& model, QWidget* parent )
    : QDialog( parent ), ui( new Ui::TransferDialog ), model_( model ), transfer_( transfer ) {
    ui->setupUi( this );
    setWindowFlags( windowFlags() & ~Qt::WindowContextHelpButtonHint );

    ui->calendarWidget->setSelectedDate( transfer->date() );
    ui->descriptionEdit->setText( transfer->description() );
    ui->privateEdit->setText( transfer->privateDescription() );
    ui->amountSpinBox->setValue( transfer->cents() / 100.0 );

    auto completer = new QCompleter( model.completions(), this );
    ui->descriptionEdit->setCompleter( completer );

    ui->internalAccountBox->addItem( "Verteilt - Alle", 0 );
    ui->internalAccountBox->addItem( "Verteilt - Alle mit Guthaben", 1 );
    size_t shareCount = 0;
    size_t externalShareId = 0;
    size_t internalShareId = 0;
    for( const auto& account : model.accounts() ) {
        auto id = reinterpret_cast< size_t >( account.get() );
        if( test( account->flags(), Flags::External ) ) {
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

    if( externalShareId != 0 ) {
        ui->externalAccountBox->setCurrentIndex( ui->externalAccountBox->findData( externalShareId ) );
    } else {
        ui->externalAccountBox->setCurrentIndex( 0 );
    }

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

    if( external == nullptr ) {
        return;
    }

    model_.remove( transfer_ );
    auto transfer = std::make_shared< Transfer >( ui->calendarWidget->selectedDate(),
                                                  ui->descriptionEdit->text(),
                                                  round( ui->amountSpinBox->value() * 100.0 ) );
    transfer->setPrivateDescription( ui->privateEdit->text() );
    if( ui->privateEdit->text().isEmpty() ) {
        transfer->setPrivateDescription( ui->privateEdit->placeholderText() );
    }

    if( internal != nullptr ) {
        model_.insert( transfer, external, internal );
    } else if( ui->internalAccountBox->currentData() == 0 ) {
        model_.insert( transfer, external, Model::GroupInsertMode::AllIndividual );
    } else if( ui->internalAccountBox->currentData() == 1 ) {
        model_.insert( transfer, external, Model::GroupInsertMode::NonNegativeIndividual );
    }

    QDialog::accept();
}

void TransferDialog::on_deleteButton_clicked() {
    model_.remove( transfer_ );
    QDialog::accept();
}

void TransferDialog::on_internalAccountBox_currentTextChanged( const QString& text ) {
    if( ui->internalAccountBox->currentData().toULongLong() > 1 ) {
        ui->privateEdit->setPlaceholderText( text );
    }
}
