#include "Account.h"

#include <gtest/gtest.h>

TEST( AccountTest, ShouldCalculateBalance ) {
    Account account( "MyAccount", Account::Type::Internal );

    account.addTransfer( std::make_shared< Transfer >( QDate(), "", 314 ), 0.75 );

    auto transfer = std::make_shared< Transfer >( QDate(), "", 586 );
    account.addTransfer( transfer, 0.33 );
    account.addTransfer( transfer, 1 );

    ASSERT_EQ( 1014, account.balance() );

    account.removeTransfer( transfer );
    ASSERT_EQ( 235, account.balance() );
}
