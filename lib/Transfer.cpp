#include "Transfer.h"

Transfer::Transfer( QDate date, QString desc, const Amount& amount )
    : date_( date ), description_( desc ), amount_( amount ) {
}

const QDate& Transfer::date() const {
    return date_;
}

const QString& Transfer::description() const {
    return description_;
}

const Amount& Transfer::amount() const {
    return amount_;
}

void Transfer::setDate( const QDate& date ) {
    date_ = date;
}

void Transfer::setDescription( const QString& description ) {
    description_ = description;
}

void Transfer::setAmount( const Amount& amount ) {
    amount_ = amount;
}
