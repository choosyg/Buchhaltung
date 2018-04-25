#pragma once

#include "Account.h"
#include "Transfer.h"

#include <QDialog>

namespace Ui {
class TransferDialog;
}

class TransferDialog : public QDialog {
    Q_OBJECT

public:
    explicit TransferDialog( TransferPtr transfer, QList< AccountPtr >& accounts, QWidget* parent = 0 );
    ~TransferDialog();

    void accept() override;

private:
    Ui::TransferDialog* ui;
    TransferPtr transfer_;
    QList< AccountPtr >& accounts_;
};
