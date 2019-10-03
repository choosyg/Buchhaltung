#pragma once

#include "AccountOverviewTableModel.h"
#include "Model.h"

#include <QMainWindow>
#include <QSortFilterProxyModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow( QWidget* parent = nullptr );
    ~MainWindow() override;

private slots:
    void on_openButton_clicked();
    void on_saveButton_clicked();

    void on_externalOverviewTable_doubleClicked( const QModelIndex& index );
    void on_internalOverviewTable_doubleClicked( const QModelIndex& index );

    void on_spreadButton_clicked();
    void on_newTransferbutton_clicked();
    void on_newInternalTransferButton_clicked();

    void on_printButton_clicked();

private:
    Ui::MainWindow* ui;
    Model model_;
    int divisionRest_;

    QSortFilterProxyModel externalSortModel_;
    AccountOverviewTableModel externalOverviewTableModel_;
    QSortFilterProxyModel internalSortModel_;
    AccountOverviewTableModel internalOverviewTableModel_;

    void updateViews();
};
