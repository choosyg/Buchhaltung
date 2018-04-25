#pragma once

#include "Amount.h"

#include <QDate>

#include <memory>

class Transfer {
public:
    explicit Transfer( QDate date, QString desc, const Amount& amount );

    const QDate& date() const;
    const QString& description() const;
    const Amount& amount() const;

    void setDate( const QDate& date );
    void setDescription( const QString& description );
    void setAmount( const Amount& amount );

private:
    QDate date_;
    QString description_;
    Amount amount_;
};

typedef std::shared_ptr< Transfer > TransferPtr;
