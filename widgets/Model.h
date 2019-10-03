#pragma once

#include <core/Account.h>

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
    void insertInternal( TransferConstPtr transfer, AccountConstPtr source, AccountConstPtr target );

    enum class GroupInsertMode { AllIndividual, NonNegativeIndividual };
    void insert( TransferConstPtr transfer, AccountConstPtr external, GroupInsertMode mode );

    const QStringList& completions() const;

private:
    mutable QList< AccountPtr > accounts_;
    QStringList completions_;
};
