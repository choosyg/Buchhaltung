#include "PrintPreviewDialog.h"
#include "ui_PrintPreviewDialog.h"

PrintPreviewDialog::PrintPreviewDialog( QList< AccountPtr >& accounts, QWidget* parent )
    : QDialog( parent ), ui( new Ui::PrintPreviewDialog ), accounts_( accounts ) {
    ui->setupUi( this );
    for( int year = 2016; year <= QDate::currentDate().year(); ++year ) {
        ui->comboBox->addItem( QString::number( year ), year );
    }
    ui->comboBox->setCurrentIndex( ui->comboBox->findData( QDate::currentDate().year() ) );
}

PrintPreviewDialog::~PrintPreviewDialog() {
    delete ui;
}

void PrintPreviewDialog::on_comboBox_currentIndexChanged( int index ) {
    ui->textEdit->clear();

    auto year = ui->comboBox->currentData().toInt();

    for( const auto& account : accounts_ ) {
        ui->textEdit->append( getReport( account, year ) );
    }
}

QString PrintPreviewDialog::getReport( const AccountPtr& account, int year ) const {
    Amount begin;
    Amount end;
    for( const auto& share : account->transferShares() ) {
        if( share->transfer()->date().year() < year ) {
            begin += share->amount();
        }
        if( share->transfer()->date().year() <= year ) {
            end += share->amount();
        }
    }

    QString report;
    report += "Konto: " + account->name() + "\n";
    report += "Kontostand am 31.12." + QString::number( year - 1 ) + ": " + begin.toString() + "\n";

    for( const auto& share : account->transferShares() ) {
        if( share->transfer()->date().year() == year ) {
            report += "\t" + share->transfer()->date().toString( "yyyy-MM-dd" ) + "  "
                      + share->transfer()->description() + "  " + share->amount().toString() + "\n";
        }
    }

    report += "Kontostand am 31.12." + QString::number( year ) + ": " + end.toString() + "\n";
    return report;
}
