#pragma once

#include "Transfer.h"

class TransferShare {
public:
    explicit TransferShare( TransferPtr transfer, double share );

    Amount amount() const;
    TransferPtr transfer() const;
    double share() const;

private:
    TransferPtr transfer_;
    double share_;
};

typedef std::shared_ptr< TransferShare > TransferSharePtr;
