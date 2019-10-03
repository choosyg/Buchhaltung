#pragma once

#include <QDate>

#include <memory>

class Transfer {
public:
    explicit Transfer( QDate date, QString desc, int cents );

    const QDate& date() const;
    const QString& description() const;
    const QString& privateDescription() const;
    int cents() const;

    void setDate( const QDate& date );
    void setDescription( const QString& description );
    void setPrivateDescription( const QString& privateDescription );
    void setCents( int cents );

private:
    QDate date_;
    QString description_;
    QString privateDescription_;
    int cents_;
};
typedef std::shared_ptr< Transfer > TransferPtr;
typedef std::shared_ptr< const Transfer > TransferConstPtr;

QString formatCents( int cents );
