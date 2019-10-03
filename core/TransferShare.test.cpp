#include "TransferShare.h"

#include <gtest/gtest.h>

TEST( TransferShareTest, ShouldRoundToCorrectDirection ) {
    TransferPtr t = std::make_shared< Transfer >( QDate(), "", 100 );
    ASSERT_EQ( 33, TransferShare( t, 1.0 / 3.0 ).cents() );

    TransferPtr m = std::make_shared< Transfer >( QDate(), "", -100 );
    ASSERT_EQ( -34, TransferShare( m, 1.0 / 3.0 ).cents() );
}
