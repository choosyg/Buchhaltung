#pragma once

#include "TransferShare.h"

#include <vector>

class Account {
public:
    enum class Type { Internal, External };

    Account( QString name, Type type );

    Type type() const;
    const QString& name() const;

    void addTransfer( TransferConstPtr transfer, double share = 1.0 );
    void removeTransfer( TransferConstPtr transfer );
    int balance() const;

    const std::vector< TransferShareConstPtr >& transferShares() const;
    bool shares( const TransferConstPtr& transfer ) const;

private:
    QString name_;
    Type type_;
    std::vector< TransferShareConstPtr > transfers_;
};

typedef std::shared_ptr< Account > AccountPtr;
typedef std::shared_ptr< const Account > AccountConstPtr;
