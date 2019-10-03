#pragma once

#include "Model.h"

#include <core/Transfer.h>

#include <QDialog>

namespace Ui {
class InternalTransferDialog;
}

class InternalTransferDialog : public QDialog {
    Q_OBJECT

public:
    explicit InternalTransferDialog( TransferConstPtr transfer, Model& model, QWidget* parent = nullptr );
    ~InternalTransferDialog() override;

    void accept() override;

private slots:
    void on_deleteButton_clicked();

private:
    Ui::InternalTransferDialog* ui;
    Model& model_;
    TransferConstPtr transfer_;
};
