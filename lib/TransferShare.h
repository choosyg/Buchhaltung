#pragma once

#include "Transfer.h"

class TransferShare {
public:
    explicit TransferShare( TransferConstPtr transfer, double share );

    int cents() const;
    TransferConstPtr transfer() const;
    double share() const;

private:
    TransferConstPtr transfer_;
    double share_;
};

typedef std::shared_ptr< TransferShare > TransferSharePtr;
typedef std::shared_ptr< const TransferShare > TransferShareConstPtr;
