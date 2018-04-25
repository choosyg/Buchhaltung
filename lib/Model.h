#pragma once

#include "Account.h"

class Model {
public:
    Model();

    void save( QString filename ) const;
    void load( QString filename );

    QList< TransferPtr > transfers_;
    QList< AccountPtr > accounts_;
};
