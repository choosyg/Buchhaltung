#include "Model.h"

#include <gtest/gtest.h>

TEST( ModelTest, ShouldLoad ) {
    Model model;
    model.load( QString( TEST_DATA ) + "/test.json" );
    ASSERT_EQ( 2, model.count( Flags::External ) );
    ASSERT_EQ( 3, model.count( Flags::Internal ) );
    ASSERT_EQ( 5, model.accounts().size() );
}

TEST( ModelTest, ShouldUpsertDivisionRest ) {
    Model model;
    model.load( QString( TEST_DATA ) + "/test.json" );

    auto transfer = std::make_shared< Transfer >( QDate::currentDate(), "desc", 1000 );
    model.insert( transfer, nullptr, nullptr );
    ASSERT_EQ( 999, model.sumBalance( Flags::Internal ) );
    ASSERT_EQ( 0, model.sumBalance( Flags::External ) );
}

TEST( ModelTest, ShouldUpsertCommonCosts ) {
    Model model;
    model.load( QString( TEST_DATA ) + "/test.json" );

    AccountConstPtr external = nullptr;
    for( auto& a : model.accounts() ) {
        if( test( a->flags(), Flags::External ) ) {
            external = a;
            break;
        }
    }

    auto transfer = std::make_shared< Transfer >( QDate::currentDate(), "desc", 1000 );
    model.insert( transfer, external, nullptr );
    ASSERT_EQ( 999, model.sumBalance( Flags::Internal ) );
    ASSERT_EQ( 1000, model.sumBalance( Flags::External ) );
    ASSERT_EQ( 1000, external->balance() );
}

TEST( ModelTest, ShouldUpsertIndividualCosts ) {
    Model model;
    model.load( QString( TEST_DATA ) + "/test.json" );

    AccountConstPtr external = nullptr;
    AccountConstPtr internal = nullptr;
    for( auto& a : model.accounts() ) {
        if( test( a->flags(), Flags::External ) ) {
            external = a;
        }
        if( test( a->flags(), Flags::Internal ) ) {
            internal = a;
        }
    }

    auto transfer = std::make_shared< Transfer >( QDate::currentDate(), "desc", 1000 );
    model.insert( transfer, external, internal );
    ASSERT_EQ( 1000, model.sumBalance( Flags::Internal ) );
    ASSERT_EQ( 1000, model.sumBalance( Flags::External ) );
    ASSERT_EQ( 1000, internal->balance() );
    ASSERT_EQ( 1000, external->balance() );
}

TEST( ModelTest, ShouldThrowOnUnintendedUpserts ) {
    Model model;
    model.load( QString( TEST_DATA ) + "/test.json" );

    AccountConstPtr internal = nullptr;
    for( auto& a : model.accounts() ) {
        if( test( a->flags(), Flags::Internal ) ) {
            internal = a;
        }
    }

    auto transfer = std::make_shared< Transfer >( QDate::currentDate(), "desc", 1000 );
    ASSERT_ANY_THROW( model.insert( transfer, nullptr, internal ) );
}
