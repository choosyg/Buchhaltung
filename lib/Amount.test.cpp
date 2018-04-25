#include "Transfer.h"

#include <gtest/gtest.h>

#include <QJsonDocument>

TEST( AmountTest, ShouldThrowIfTooManyDecimals ) {
    ASSERT_ANY_THROW( Amount( 1.123 ) );
}

TEST( AmountTest, ShouldAddAmounts ) {
    ASSERT_EQ( 15656.77, Amount( 5496.84 ) + Amount( 10159.93 ) );

    Amount a( 1.1 );
    Amount b( 2.04 );
    a += b;
    ASSERT_EQ( 3.14, a );
}

TEST( AmountTest, ShouldSubtractAmounts ) {
    ASSERT_EQ( 1.1, Amount( 3.14 ) - Amount( 2.04 ) );

    Amount a( 3.14 );
    Amount b( 2.04 );
    a -= b;
    ASSERT_EQ( 1.1, a );
}
