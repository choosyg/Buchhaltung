#pragma once

#include "Account.h"

#include <QDialog>

namespace Ui {
class PrintPreviewDialog;
}

class PrintPreviewDialog : public QDialog {
    Q_OBJECT
public:
    explicit PrintPreviewDialog( QList< AccountPtr >& accounts, QWidget* parent = 0 );
    ~PrintPreviewDialog();

private slots:
    void on_comboBox_currentIndexChanged( int index );

private:
    Ui::PrintPreviewDialog* ui;
    QList< AccountPtr >& accounts_;

    QString getReport( const AccountPtr& account, int year ) const;
};
