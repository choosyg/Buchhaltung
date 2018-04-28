#include "Account.h"

Account::Account( QString name, Type type ) : name_( name ), type_( type ) {
}

Account::Type Account::type() const {
    return type_;
}

void Account::addTransfer( TransferPtr transfer, double share ) {
    transfers_.emplace_back( std::make_shared< TransferShare >( transfer, share ) );
}

void Account::removeTransfer( TransferPtr transfer ) {
    auto it = std::remove_if( transfers_.begin(), transfers_.end(), [&transfer]( const TransferSharePtr& ts ) {
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

const std::vector< TransferSharePtr >& Account::transferShares() const {
    return transfers_;
}

bool Account::shares( const TransferPtr& transfer ) const {
    for( const auto& share : transfers_ ) {
        if( share->transfer().get() == transfer.get() ) {
            return true;
        }
    }
    return false;
}

const QString& Account::name() const {
    return name_;
}
