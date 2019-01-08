#pragma once

#include "TransferShare.h"

#include <vector>

enum class Flags { Internal = 1, External = 2, Group = 4, Individual = 8 };
Flags operator|( Flags a, Flags b );
bool test( Flags flags, Flags contained );

class Account {
public:
    Account( QString name, Flags flags );

    Flags flags() const;
    const QString& name() const;
    const QString& adress() const;

    void setAdress( const QString& adress );
    void setName( const QString& name );

    void addTransfer( TransferConstPtr transfer, double share = 1.0 );
    void removeTransfer( TransferConstPtr transfer );
    int balance() const;
    std::pair< int, int > balance( int year ) const;

    const std::vector< TransferShareConstPtr >& transferShares() const;
    bool shares( const TransferConstPtr& transfer ) const;
    double share( const TransferConstPtr& transfer ) const;

private:
    QString name_;
    QString adress_;
    Flags flags_;
    std::vector< TransferShareConstPtr > transfers_;
};

typedef std::shared_ptr< Account > AccountPtr;
typedef std::shared_ptr< const Account > AccountConstPtr;
