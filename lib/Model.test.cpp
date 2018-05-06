#include "Model.h"

#include <gtest/gtest.h>

TEST( ModelTest, ShouldLoad ) {
    Model model;
    model.load( QString( TEST_DATA ) + "/test.json" );
    ASSERT_EQ( 2, model.count( Account::Type::External ) );
    ASSERT_EQ( 3, model.count( Account::Type::Internal ) );
    ASSERT_EQ( 5, model.accounts().size() );
}

TEST( ModelTest, ShouldUpsertDivisionRest ) {
    Model model;
    model.load( QString( TEST_DATA ) + "/test.json" );

    auto transfer = std::make_shared< Transfer >( QDate::currentDate(), "desc", 1000 );
    model.upsert( transfer, nullptr, nullptr );
    ASSERT_EQ( 999, model.sumBalance( Account::Type::Internal ) );
    ASSERT_EQ( 0, model.sumBalance( Account::Type::External ) );
}

TEST( ModelTest, ShouldUpsertCommonCosts ) {
    Model model;
    model.load( QString( TEST_DATA ) + "/test.json" );

    AccountConstPtr external = nullptr;
    for( auto& a : model.accounts() ) {
        if( a->type() == Account::Type::External ) {
            external = a;
            break;
        }
    }

    auto transfer = std::make_shared< Transfer >( QDate::currentDate(), "desc", 1000 );
    model.upsert( transfer, external, nullptr );
    ASSERT_EQ( 999, model.sumBalance( Account::Type::Internal ) );
    ASSERT_EQ( 1000, model.sumBalance( Account::Type::External ) );
    ASSERT_EQ( 1000, external->balance() );
}

TEST( ModelTest, ShouldUpsertIndividualCosts ) {
    Model model;
    model.load( QString( TEST_DATA ) + "/test.json" );

    AccountConstPtr external = nullptr;
    AccountConstPtr internal = nullptr;
    for( auto& a : model.accounts() ) {
        if( a->type() == Account::Type::External ) {
            external = a;
        }
        if( a->type() == Account::Type::Internal ) {
            internal = a;
        }
    }

    auto transfer = std::make_shared< Transfer >( QDate::currentDate(), "desc", 1000 );
    model.upsert( transfer, external, internal );
    ASSERT_EQ( 1000, model.sumBalance( Account::Type::Internal ) );
    ASSERT_EQ( 1000, model.sumBalance( Account::Type::External ) );
    ASSERT_EQ( 1000, internal->balance() );
    ASSERT_EQ( 1000, external->balance() );
}
