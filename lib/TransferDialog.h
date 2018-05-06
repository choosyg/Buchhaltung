#pragma once

#include "Model.h"
#include "Transfer.h"

#include <QDialog>

namespace Ui {
class TransferDialog;
}

class TransferDialog : public QDialog {
    Q_OBJECT

public:
    explicit TransferDialog( TransferConstPtr transfer, Model& model, QWidget* parent = 0 );
    ~TransferDialog();

    void accept() override;

private:
    Ui::TransferDialog* ui;
    Model& model_;
};
