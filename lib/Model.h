#pragma once

#include "Account.h"

class Model {
public:
    Model();

    void save( QString filename ) const;
    void load( QString filename );

    int count( Account::Type type ) const;
    int sumBalance( Account::Type type ) const;

    QList< AccountConstPtr > accounts() const;

    void upsert( TransferConstPtr transfer, AccountConstPtr external, AccountConstPtr internal );

private:
    mutable QList< AccountPtr > accounts_;
};
