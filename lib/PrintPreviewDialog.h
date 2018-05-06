#pragma once

#include "Account.h"

#include <QDialog>

namespace Ui {
class PrintPreviewDialog;
}

class PrintPreviewDialog : public QDialog {
    Q_OBJECT
public:
    explicit PrintPreviewDialog( const QList< AccountConstPtr >& accounts, QWidget* parent = 0 );
    ~PrintPreviewDialog();

private slots:
    void on_comboBox_currentIndexChanged( int index );

    void on_toolButton_clicked();

private:
    Ui::PrintPreviewDialog* ui;
    const QList< AccountConstPtr >& accounts_;

    QString buildReport( const AccountConstPtr& account, int year ) const;
    QStringList buildPages();
};
