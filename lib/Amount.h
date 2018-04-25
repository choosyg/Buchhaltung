#pragma once

#include <QDate>

#include <memory>

class Amount {
public:
    explicit Amount( double amount = 0.0 );
    Amount( const Amount& other ) = default;

    Amount operator+( const Amount& other ) const;
    Amount operator-( const Amount& other ) const;
    Amount& operator+=( const Amount& other );
    Amount& operator-=( const Amount& other );

    double amount() const;
    QString toString() const;

private:
    double amount_;
};

bool operator==( const Amount& a, const Amount& b );
bool operator==( const Amount& a, double b );
bool operator==( double a, const Amount& b );
