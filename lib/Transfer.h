#pragma once

#include <QDate>

#include <memory>

class Transfer {
public:
    explicit Transfer( QDate date, QString desc, int cents );

    const QDate& date() const;
    const QString& description() const;
    int cents() const;

    void setDate( const QDate& date );
    void setDescription( const QString& description );
    void setCens( int cents );

private:
    QDate date_;
    QString description_;
    int cents_;
};
typedef std::shared_ptr< Transfer > TransferPtr;

QString formatCents( int cents );
