#include "Account.h"

#include <type_traits>

Account::Account( QString name, Flags flags ) : name_( name ), flags_( flags ) {
    if( !test( flags, Flags::Internal ) && !test( flags, Flags::External ) ) {
        throw std::invalid_argument( "Account::C'Tor neither internal nor external account" );
    }
    if( test( flags, Flags::Internal ) && test( flags, Flags::External ) ) {
        throw std::invalid_argument( "Account::C'Tor internal AND external account" );
    }
    if( !test( flags, Flags::Individual ) && !test( flags, Flags::Group ) ) {
        throw std::invalid_argument( "Account::C'Tor neither group nor individual account" );
    }
    if( test( flags, Flags::Individual ) && test( flags, Flags::Group ) ) {
        throw std::invalid_argument( "Account::C'Tor group AND individual account" );
    }
}

Flags Account::flags() const {
    return flags_;
}

void Account::addTransfer( TransferConstPtr transfer, double share ) {
    transfers_.emplace_back( std::make_shared< TransferShare >( transfer, share ) );
}

void Account::removeTransfer( TransferConstPtr transfer ) {
    auto it = std::remove_if( transfers_.begin(), transfers_.end(), [&transfer]( const TransferShareConstPtr& ts ) {
        return ts->transfer().get() == transfer.get();
    } );
    transfers_.erase( it, transfers_.end() );
}

int Account::balance() const {
    int total = 0;
    for( const auto& share : transfers_ ) {
        total += share->cents();
    }
    return total;
}

const std::vector< TransferShareConstPtr >& Account::transferShares() const {
    return transfers_;
}

bool Account::shares( const TransferConstPtr& transfer ) const {
    for( const auto& share : transfers_ ) {
        if( share->transfer().get() == transfer.get() ) {
            return true;
        }
    }
    return false;
}

void Account::setAdress( const QString& adress ) {
    adress_ = adress;
}

void Account::setName( const QString& name ) {
    name_ = name;
}

const QString& Account::adress() const {
    return adress_;
}

const QString& Account::name() const {
    return name_;
}

Flags operator|( Flags a, Flags b ) {
    using ut = std::underlying_type< Flags >::type;
    ut at = static_cast< ut >( a );
    ut bt = static_cast< ut >( b );
    return static_cast< Flags >( at | bt );
}

bool test( Flags flags, Flags contained ) {
    using ut = std::underlying_type< Flags >::type;
    ut at = static_cast< ut >( flags );
    ut bt = static_cast< ut >( contained );
    return ( at & bt ) == bt;
}
