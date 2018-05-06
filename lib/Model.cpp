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
        obj["cents"] = transfer->cents();
        obj["id"] = QString::number( reinterpret_cast< size_t >( transfer.get() ) );
        transfers.append( obj );
    }

    QJsonArray accounts;
    for( const auto& account : accounts_ ) {
        QJsonObject a;
        a["name"] = account->name();
        a["type"] = account->type() == Account::Type::Internal ? "Internal" : "External";
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
    global["accouns"] = accounts;

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
        idToTransfer[obj["id"].toString()]
            = std::make_shared< Transfer >( QDate::fromString( obj["date"].toString(), "yyyy-MM-dd" ),
                                            obj["description"].toString(),
                                            obj["cents"].toInt() );
    }

    QJsonArray accounts = document.object()["accounts"].toArray();
    for( const auto& a : accounts ) {
        QJsonObject obj = a.toObject();
        accounts_.append( std::make_shared< Account >( obj["name"].toString(),
                                                       obj["type"].toString() == "Internal"
                                                           ? Account::Type::Internal
                                                           : Account::Type::External ) );
        QJsonArray shares = obj["shares"].toArray();
        for( const auto& s : shares ) {
            QJsonObject share = s.toObject();
            accounts_.back()->addTransfer( idToTransfer[share["id"].toString()], share["share"].toDouble() );
        }
    }
}

int Model::count( Account::Type type ) const {
    int count = 0;
    for( const auto& account : accounts_ ) {
        if( account->type() == type ) {
            ++count;
        }
    }
    return count;
}

int Model::sumBalance( Account::Type type ) const {
    int sum = 0;
    for( const auto& account : accounts_ ) {
        if( account->type() == type ) {
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

void Model::upsert( TransferConstPtr transfer, AccountConstPtr external, AccountConstPtr internal ) {
    if( external == nullptr && internal != nullptr ) {
        throw std::exception( "Model::upsert: Tried to upsert transfer without external reference" );
    }
    for( const auto& account : accounts_ ) {
        account->removeTransfer( transfer );
    }

    if( external != nullptr ) {
        for( const auto& account : accounts_ ) {
            if( account.get() == external.get() ) {
                account->addTransfer( transfer );
            }
        }
    }

    if( internal != nullptr ) {
        for( const auto& account : accounts_ ) {
            if( account.get() == internal.get() ) {
                account->addTransfer( transfer );
            }
        }
    } else {
        size_t ic = count( Account::Type::Internal );
        for( const auto& account : accounts_ ) {
            if( account->type() == Account::Type::Internal ) {
                account->addTransfer( transfer, 1.0 / ic );
            }
        }
    }
}
