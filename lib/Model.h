#pragma once

#include "Account.h"

class Model {
public:
    Model();

    void save( QString filename ) const;
    void load( QString filename );

    int count( Flags flags ) const;
    int sumBalance( Flags flags ) const;

    QList< AccountConstPtr > accounts() const;

    void remove( TransferConstPtr transfer );
    void insert( TransferConstPtr transfer, AccountConstPtr external, AccountConstPtr internal );

    enum class GroupInsertMode { AllIndividual, NonNegativeIndividual };
    void insert( TransferConstPtr transfer, AccountConstPtr external, GroupInsertMode mode );

private:
    mutable QList< AccountPtr > accounts_;
};
