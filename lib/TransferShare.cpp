#include "TransferShare.h"

TransferShare::TransferShare( TransferPtr transfer, double share ) : transfer_( transfer ), share_( share ) {
}

Amount TransferShare::amount() const {
    double value = transfer_->amount().amount() * share_;
    return Amount( std::floor( value * 100.0 ) / 100.0 );
}

TransferPtr TransferShare::transfer() const {
    return transfer_;
}

double TransferShare::share() const {
    return share_;
}
