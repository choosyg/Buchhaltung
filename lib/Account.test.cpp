#include "Account.h"

#include <gtest/gtest.h>

TEST( AccountTest, ShouldCalculateBalance ) {
    Account account( "MyAccount", Account::Type::Internal );

    account.addTransfer( std::make_shared< Transfer >( QDate(), "", Amount( 3.14 ) ), 0.75 );

    auto transfer = std::make_shared< Transfer >( QDate(), "", Amount( 5.86 ) );
    account.addTransfer( transfer, 0.33 );
    account.addTransfer( transfer, 1 );

    ASSERT_EQ( 10.14, account.balance() );

    account.removeTransfer( transfer );
    ASSERT_EQ( 2.35, account.balance() );
}
