#include "Transfer.h"

Transfer::Transfer( QDate date, QString desc, int cents ) : date_( date ), description_( desc ), cents_( cents ) {
}

const QDate& Transfer::date() const {
    return date_;
}

const QString& Transfer::description() const {
    return description_;
}

int Transfer::cents() const {
    return cents_;
}

void Transfer::setDate( const QDate& date ) {
    date_ = date;
}

void Transfer::setDescription( const QString& description ) {
    description_ = description;
}

void Transfer::setCens( int cents ) {
    cents_ = cents;
}

QString formatCents( int cents ) {
    return QString().sprintf( "%+.2f€", cents / 100.0 );
}
