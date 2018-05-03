#include "PrintPreviewDialog.h"
#include "ui_PrintPreviewDialog.h"

#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrinter>
#include <QPainter>

PrintPreviewDialog::PrintPreviewDialog( QList< AccountPtr >& accounts, QWidget* parent )
    : QDialog( parent ), ui( new Ui::PrintPreviewDialog ), accounts_( accounts ) {
    ui->setupUi( this );
    setWindowFlags( windowFlags() & ~Qt::WindowContextHelpButtonHint );

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
        QString report = buildReport( account, year );
        ui->textEdit->append( report );
    }
}

QString PrintPreviewDialog::buildReport( const AccountPtr& account, int year ) const {
    int begin{0};
    int end{0};
    for( const auto& share : account->transferShares() ) {
        if( share->transfer()->date().year() < year ) {
            begin += share->cents();
        }
        if( share->transfer()->date().year() <= year ) {
            end += share->cents();
        }
    }

    QString report;
    report += "<p><b><u>" + account->name() + "</u></b><br/>";
    report += "Kontostand am 31.12." + QString::number( year - 1 ) + ": " + formatCents( begin ) + "<br/>";

    for( const auto& share : account->transferShares() ) {
        if( share->transfer()->date().year() == year ) {
            report += "\t" + share->transfer()->date().toString( "yyyy-MM-dd" ) + "  "
                      + share->transfer()->description() + "  " + formatCents( share->cents() ) + "<br/>";
        }
    }

    report += "Kontostand am 31.12." + QString::number( year ) + ": " + formatCents( end ) + "<br/></p>";
    return report;
}

QStringList PrintPreviewDialog::buildPages() {
    auto year = ui->comboBox->currentData().toInt();
    QString external;
    for( const auto& account : accounts_ ) {
        if( account->type() == Account::Type::External ) {
            external += buildReport( account, year );
        }
    }

    QStringList pageContent;
    for( const auto& account : accounts_ ) {
        if( account->type() == Account::Type::External ) {
            continue;
        }
        QString report = external;
        report += buildReport( account, year );
        pageContent.append( report );
    }

    return pageContent;
}

void PrintPreviewDialog::on_toolButton_clicked() {
    QStringList pageContent = buildPages();

    QPrinter printer;
    QPagedPaintDevice::Margins margins;
    margins.left = 15;
    margins.right = 15;
    margins.top = 20;
    margins.bottom = 20;
    printer.setMargins( margins );
    QPrintPreviewDialog preview( &printer, this );
    preview.setWindowFlags( preview.windowFlags() & ~Qt::WindowContextHelpButtonHint );
    connect( &preview, &QPrintPreviewDialog::paintRequested, this, [&]( QPrinter* printer ) {
        QPainter painter( printer );
        for( const auto& pc : pageContent ) {
            QTextDocument doc;
            doc.setHtml( pc );
            doc.drawContents( &painter );
            printer->newPage();
        }
    } );
    preview.exec();
}
