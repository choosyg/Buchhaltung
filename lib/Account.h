#pragma once

#include "TransferShare.h"

#include <vector>

class Account {
public:
    enum class Type { Internal, External };

    Account( QString name, Type type );

    Type type() const;
    const QString& name() const;

    void addTransfer( TransferPtr transfer, double share = 1.0 );
    void removeTransfer( TransferPtr transfer );
    Amount balance() const;

    const std::vector< TransferSharePtr >& transferShares() const;
    bool shares( const TransferPtr& transfer ) const;

private:
    QString name_;
    Type type_;
    std::vector< TransferSharePtr > transfers_;
};

typedef std::shared_ptr< Account > AccountPtr;
