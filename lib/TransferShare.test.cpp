#include "TransferShare.h"

#include <gtest/gtest.h>

TEST( TransferShareTest, ShouldRoundToRightDirection ) {
    TransferPtr t = std::make_shared< Transfer >( QDate(), "", Amount( 1.0 ) );
    ASSERT_EQ( 0.33, TransferShare( t, 1.0 / 3.0 ).amount() );

    TransferPtr m = std::make_shared< Transfer >( QDate(), "", Amount( -1.0 ) );
    ASSERT_EQ( -0.34, TransferShare( m, 1.0 / 3.0 ).amount() );
}
