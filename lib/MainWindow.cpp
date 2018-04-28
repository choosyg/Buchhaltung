#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AccountDialog.h"
#include "PrintPreviewDialog.h"
#include "TransferDialog.h"

#include <QFileDialog>
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

void MainWindow::updateViews() {
    int internal{0};
    int external{0};
    internalOverviewTableModel_.clear();
    externalOverviewTableModel_.clear();
    for( const auto& account : model_.accounts_ ) {
        if( account->type() == Account::Type::Internal ) {
            internal += account->balance();
            internalOverviewTableModel_.addAccount( account );
        } else {
            external += account->balance();
            externalOverviewTableModel_.addAccount( account );
        }
    }

    ui->sumInternalLabel->setText( formatCents( internal ) );
    ui->sumExternalLabel->setText( formatCents( external ) );
    ui->divisionRestLabel->setText( formatCents( external - internal ) );

    ui->numberExternalLabel->setText( QString::number( externalOverviewTableModel_.rowCount( QModelIndex() ) ) );
    ui->numberInternalLabel->setText( QString::number( internalOverviewTableModel_.rowCount( QModelIndex() ) ) );

    ui->externalOverviewTable->resizeColumnsToContents();
    ui->internalOverviewTable->resizeColumnsToContents();
}

void MainWindow::on_externalOverviewTable_doubleClicked( const QModelIndex& index ) {
    AccountDialog( externalOverviewTableModel_.account( externalSortModel_.mapToSource( index ) ),
                   model_.accounts_,
                   this )
        .exec();
    updateViews();
}

void MainWindow::on_internalOverviewTable_doubleClicked( const QModelIndex& index ) {
    AccountDialog( internalOverviewTableModel_.account( internalSortModel_.mapToSource( index ) ),
                   model_.accounts_,
                   this )
        .exec();
    updateViews();
}

void MainWindow::on_spreadButton_clicked() {
}

void MainWindow::on_newTransferbutton_clicked() {
    auto transfer = std::make_shared< Transfer >( QDate::currentDate(), "", 0 );
    TransferDialog( transfer, model_.accounts_, this ).exec();
    updateViews();
}

void MainWindow::on_printButton_clicked() {
    PrintPreviewDialog( model_.accounts_, this ).exec();
}
