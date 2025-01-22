#ifndef LMASSETIDTEST_H
#define LMASSETIDTEST_H

#define BOOST_TEST_MODULE LmAssetIDTest
#include "../../../../src/Replica/SQEX/Luminous/AssetManager/LmAssetID.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(InitialValueIsZero)
{
    const auto id =
        SQEX::Luminous::AssetManager::LmAssetID::Create("data://test.ebex@");
    BOOST_CHECK_EQUAL(0, id.fullHash_);
}

#endif // LMASSETIDTEST_H
