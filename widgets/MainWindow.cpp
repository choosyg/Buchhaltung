#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AccountDialog.h"
#include "InternalTransferDialog.h"
#include "PrintPreviewDialog.h"
#include "TransferDialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QSortFilterProxyModel>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );

    externalSortModel_.setSortRole( Qt::EditRole );
    externalSortModel_.setSourceModel( &externalOverviewTableModel_ );
    ui->externalOverviewTable->setModel( &externalSortModel_ );

    internalSortModel_.setSortRole( Qt::EditRole );
    internalSortModel_.setSourceModel( &internalOverviewTableModel_ );
    ui->internalOverviewTable->setModel( &internalSortModel_ );

    setStyleSheet( "QToolButton { "
                   "border: none;"
                   "padding: 2px;"
                   "}"
                   "QToolButton:hover { "
                   "border: 1px solid #8f8f91;"
                   "border-radius: 3px;"
                   "padding: 1px;"
                   "}"
                   "QToolButton:checked { "
                   "border: 1px solid #8f8f91;"
                   "border-radius: 3px;"
                   "padding: 1px;"
                   "background-color: #cccccc;"
                   "}"
                   "QToolButton:pressed { "
                   "border: 1px solid #8f8f91;"
                   "border-radius: 3px;"
                   "padding: 1px;"
                   "background-color: #cccccc;"
                   "}"
                   "QAbstractItemView {"
                   "  alternate-background-color: #f5faff;"
                   "}"
                   "QHeaderView {"
                   "  background-color: white;"
                   "}"
                   "QHeaderView::section {"
                   "  border: none;"
                   "  border-right: 1px solid #cccccc;"
                   "  background-color: white;"
                   "}"
                   "QAbstractItemView:item:selected {"
                   "  color: black;"
                   "  background-color:#cde8ff;"
                   "}" );
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
    model_.insert( transfer, nullptr, Model::GroupInsertMode::AllIndividual );
    updateViews();
}

void MainWindow::on_newTransferbutton_clicked() {
    auto transfer = std::make_shared< Transfer >( QDate::currentDate(), "", 0 );
    TransferDialog( transfer, model_, this ).exec();
    updateViews();
}

void MainWindow::on_newInternalTransferButton_clicked() {
    auto transfer = std::make_shared< Transfer >( QDate::currentDate(), "", 0 );
    InternalTransferDialog( transfer, model_, this ).exec();
    updateViews();
}

void MainWindow::on_printButton_clicked() {
    PrintPreviewDialog( model_.accounts(), this ).exec();
}

void MainWindow::updateViews() {
    internalOverviewTableModel_.clear();
    externalOverviewTableModel_.clear();
    for( const auto& account : model_.accounts() ) {
        if( test( account->flags(), Flags::Internal ) ) {
            internalOverviewTableModel_.addAccount( account );
        } else {
            externalOverviewTableModel_.addAccount( account );
        }
    }

    int internal = model_.sumBalance( Flags::Internal );
    int external = model_.sumBalance( Flags::External );
    ui->sumInternalLabel->setText( formatCents( internal ) );
    ui->sumExternalLabel->setText( formatCents( external ) );
    ui->divisionRestLabel->setText( formatCents( external - internal ) );
    divisionRest_ = external - internal;

    ui->numberExternalLabel->setText( QString::number( externalOverviewTableModel_.rowCount( QModelIndex() ) ) );
    ui->numberInternalLabel->setText( QString::number( internalOverviewTableModel_.rowCount( QModelIndex() ) ) );

    ui->externalOverviewTable->resizeColumnsToContents();
    ui->externalOverviewTable->sortByColumn( 0, Qt::AscendingOrder );
    ui->internalOverviewTable->resizeColumnsToContents();
    ui->internalOverviewTable->sortByColumn( 0, Qt::AscendingOrder );
}
