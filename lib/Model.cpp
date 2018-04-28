#include "Model.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

Model::Model() {
}

void Model::save( QString filename ) const {

    QJsonArray transfers;
    for( const auto& transfer : transfers_ ) {
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
    transfers_.clear();

    QJsonArray transfers = document.object()["transfers"].toArray();
    QMap< QString, TransferPtr > idToTransfer;
    for( const auto& t : transfers ) {
        QJsonObject obj = t.toObject();
        transfers_.append( std::make_shared< Transfer >( QDate::fromString( obj["date"].toString(), "yyyy-MM-dd" ),
                                                         obj["description"].toString(),
                                                         obj["cents"].toInt() ) );
        idToTransfer[obj["id"].toString()] = transfers_.back();
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
