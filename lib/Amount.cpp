#include "Amount.h"

Amount::Amount( double amount ) : amount_( amount ) {
    if( std::floor( amount_ * 100.0 ) / 100.0 != amount_ ) {
        throw std::runtime_error( "Amount::c'tor called with too many decimals" );
    }
}

Amount Amount::operator+( const Amount& other ) const {
    return Amount( amount_ + other.amount_ );
}

Amount Amount::operator-( const Amount& other ) const {
    return Amount( amount_ - other.amount_ );
}

Amount& Amount::operator+=( const Amount& other ) {
    amount_ += other.amount();
    return *this;
}

Amount& Amount::operator-=( const Amount& other ) {
    amount_ -= other.amount();
    return *this;
}

double Amount::amount() const {
    return amount_;
}

QString Amount::toString() const {
    return QString().sprintf( "%+.2f€", amount_ );
}

bool operator==( const Amount& a, const Amount& b ) {
    return a.amount() == b.amount();
}

bool operator==( const Amount& a, double b ) {
    return a.amount() == b;
}

bool operator==( double a, const Amount& b ) {
    return a == b.amount();
}
