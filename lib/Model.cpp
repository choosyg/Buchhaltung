#include "Model.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

Model::Model() {
}

void Model::save( QString filename ) const {

    std::map< size_t, TransferConstPtr > transferMap;
    for( const auto& account : accounts_ ) {
        for( const auto& share : account->transferShares() ) {
            auto id = reinterpret_cast< size_t >( share->transfer().get() );
            transferMap[id] = share->transfer();
        }
    }

    QJsonArray transfers;
    for( const auto& p : transferMap ) {
        auto transfer = p.second;
        QJsonObject obj;
        obj["date"] = transfer->date().toString( "yyyy-MM-dd" );
        obj["description"] = transfer->description();
        obj["private"] = transfer->privateDescription();
        obj["cents"] = transfer->cents();
        obj["id"] = QString::number( reinterpret_cast< size_t >( transfer.get() ) );
        transfers.append( obj );
    }

    QJsonArray accounts;
    for( const auto& account : accounts_ ) {
        QJsonObject a;
        a["name"] = account->name();
        a["adress"] = account->adress();
        a["flags"] = static_cast< int >( account->flags() );
        QJsonArray shares;
        for( const auto& share : account->transferShares() ) {
            QJsonObject s;
            s["id"] = QString::number( reinterpret_cast< size_t >( share->transfer().get() ) );
            ;
            s["share"] = share->share();
            shares.append( s );
        }
        a["shares"] = shares;
        accounts.append( a );
    }

    QJsonObject global;
    global["transfers"] = transfers;
    global["accounts"] = accounts;

    QJsonDocument document;
    document.setObject( global );

    QFile file;
    file.setFileName( filename );
    file.open( QIODevice::WriteOnly | QIODevice::Text );
    file.write( document.toJson() );
    file.close();
}

void Model::load( QString filename ) {
    QFile file;
    file.setFileName( filename );
    file.open( QIODevice::ReadOnly | QIODevice::Text );
    QJsonDocument document = QJsonDocument::fromJson( QString( file.readAll() ).toUtf8() );
    file.close();

    accounts_.clear();

    QJsonArray transfers = document.object()["transfers"].toArray();
    QMap< QString, TransferPtr > idToTransfer;
    for( const auto& t : transfers ) {
        QJsonObject obj = t.toObject();
        TransferPtr t = std::make_shared< Transfer >( QDate::fromString( obj["date"].toString(), "yyyy-MM-dd" ),
                                                      obj["description"].toString(),
                                                      obj["cents"].toInt() );
        t->setPrivateDescription( obj["private"].toString() );
        idToTransfer[obj["id"].toString()] = t;

        completions_.append( obj["description"].toString() );
    }
    completions_.sort();
    completions_.removeDuplicates();

    QJsonArray accounts = document.object()["accounts"].toArray();
    for( const auto& a : accounts ) {
        QJsonObject obj = a.toObject();
        accounts_.append(
            std::make_shared< Account >( obj["name"].toString(), static_cast< Flags >( obj["flags"].toInt() ) ) );
        accounts_.back()->setAdress( obj["adress"].toString() );

        QJsonArray shares = obj["shares"].toArray();
        for( const auto& s : shares ) {
            QJsonObject share = s.toObject();
            accounts_.back()->addTransfer( idToTransfer[share["id"].toString()], share["share"].toDouble() );
        }
    }
}

int Model::count( Flags flags ) const {
    int count = 0;
    for( const auto& account : accounts_ ) {
        if( test( account->flags(), flags ) ) {
            ++count;
        }
    }
    return count;
}

int Model::sumBalance( Flags flags ) const {
    int sum = 0;
    for( const auto& account : accounts_ ) {
        if( test( account->flags(), flags ) ) {
            sum += account->balance();
        }
    }
    return sum;
}

QList< AccountConstPtr > Model::accounts() const {
    QList< AccountConstPtr > list;
    for( auto& a : accounts_ ) {
        list.append( a );
    }
    return list;
}

void Model::remove( TransferConstPtr transfer ) {
    for( const auto& account : accounts_ ) {
        account->removeTransfer( transfer );
    }
}

void Model::insert( TransferConstPtr transfer, AccountConstPtr external, AccountConstPtr internal ) {
    if( external == nullptr ) {
        throw std::exception( "Model::insert: Tried to insert transfer without external reference" );
    }
    if( internal == nullptr ) {
        throw std::exception( "Model::insert: Tried to insert transfer without internal reference" );
    }

    remove( transfer );

    for( const auto& account : accounts_ ) {
        if( account.get() == external.get() ) {
            account->addTransfer( transfer );
        }
        if( account.get() == internal.get() ) {
            account->addTransfer( transfer );
        }
    }
}

void Model::insert( TransferConstPtr transfer, AccountConstPtr external, Model::GroupInsertMode mode ) {
    remove( transfer );

    for( const auto& account : accounts_ ) {
        if( account.get() == external.get() ) {
            account->addTransfer( transfer );
        }
    }

    if( mode == GroupInsertMode::AllIndividual ) {
        size_t ic = count( Flags::Internal | Flags::Individual );
        for( const auto& account : accounts_ ) {
            if( test( account->flags(), Flags::Internal | Flags::Individual ) ) {
                account->addTransfer( transfer, 1.0 / ic );
            }
        }
    } else if( mode == GroupInsertMode::NonNegativeIndividual ) {
        size_t ic = 0;
        for( const auto& account : accounts_ ) {
            if( test( account->flags(), Flags::Internal | Flags::Individual ) && account->balance() >= 0 ) {
                ++ic;
            }
        }
        for( const auto& account : accounts_ ) {
            if( test( account->flags(), Flags::Internal | Flags::Individual ) && account->balance() >= 0 ) {
                account->addTransfer( transfer, 1.0 / ic );
            }
        }
    }
}

const QStringList& Model::completions() const {
    return completions_;
}
