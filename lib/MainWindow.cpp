#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AccountDialog.h"
#include "PrintPreviewDialog.h"
#include "TransferDialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QSortFilterProxyModel>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );

    externalSortModel_.setSourceModel( &externalOverviewTableModel_ );
    ui->externalOverviewTable->setModel( &externalSortModel_ );

    internalSortModel_.setSourceModel( &internalOverviewTableModel_ );
    ui->internalOverviewTable->setModel( &internalSortModel_ );
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_openButton_clicked() {
    QSettings settings( QCoreApplication::applicationDirPath() + "\\config.ini", QSettings::IniFormat );
    QString path = settings.value( "LastPath", "" ).toString();
    auto filename = QFileDialog::getOpenFileName( this, "", path, "Supported Files (*.json)" );
    if( filename.isEmpty() ) {
        return;
    }
    model_.load( filename );
    updateViews();

    settings.setValue( "LastPath", QFileInfo( filename ).path() );
}

void MainWindow::on_saveButton_clicked() {
    QSettings settings( QCoreApplication::applicationDirPath() + "\\config.ini", QSettings::IniFormat );
    QString path = settings.value( "LastPath", "" ).toString();
    auto filename = QFileDialog::getSaveFileName( this, "", path, "Supported Files (*.json)" );
    if( filename.isEmpty() ) {
        return;
    }
    model_.save( filename );

    settings.setValue( "LastPath", QFileInfo( filename ).path() );
}

void MainWindow::on_externalOverviewTable_doubleClicked( const QModelIndex& index ) {
    AccountDialog( externalOverviewTableModel_.account( externalSortModel_.mapToSource( index ) ), model_, this )
        .exec();
    updateViews();
}

void MainWindow::on_internalOverviewTable_doubleClicked( const QModelIndex& index ) {
    AccountDialog( internalOverviewTableModel_.account( internalSortModel_.mapToSource( index ) ), model_, this )
        .exec();
    updateViews();
}

void MainWindow::on_spreadButton_clicked() {
    auto ans
        = QMessageBox::question( this, "Sind sie sicher?", "Soll der Divisionsrest jetzt wirklich verteilt werden?" );
    if( ans == QMessageBox::No ) {
        return;
    }
    auto transfer = std::make_shared< Transfer >( QDate::currentDate(), "Divisionsrest", divisionRest_ );
    model_.upsert( transfer, nullptr, nullptr );
    updateViews();
}

void MainWindow::on_newTransferbutton_clicked() {
    auto transfer = std::make_shared< Transfer >( QDate::currentDate(), "", 0 );
    TransferDialog( transfer, model_, this ).exec();
    updateViews();
}

void MainWindow::on_printButton_clicked() {
    PrintPreviewDialog( model_.accounts(), this ).exec();
}

void MainWindow::updateViews() {
    internalOverviewTableModel_.clear();
    externalOverviewTableModel_.clear();
    for( const auto& account : model_.accounts() ) {
        if( account->type() == Account::Type::Internal ) {
            internalOverviewTableModel_.addAccount( account );
        } else {
            externalOverviewTableModel_.addAccount( account );
        }
    }

    int internal = model_.sumBalance( Account::Type::Internal );
    int external = model_.sumBalance( Account::Type::External );
    ui->sumInternalLabel->setText( formatCents( internal ) );
    ui->sumExternalLabel->setText( formatCents( external ) );
    ui->divisionRestLabel->setText( formatCents( external - internal ) );
    divisionRest_ = external - internal;

    ui->numberExternalLabel->setText( QString::number( externalOverviewTableModel_.rowCount( QModelIndex() ) ) );
    ui->numberInternalLabel->setText( QString::number( internalOverviewTableModel_.rowCount( QModelIndex() ) ) );

    ui->externalOverviewTable->resizeColumnsToContents();
    ui->internalOverviewTable->resizeColumnsToContents();
}
