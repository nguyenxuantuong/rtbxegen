#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <functional>

#include <gtest/gtest.h>
#include <folly/json.h>
#include <folly/io/IOBuf.h>

#include <boost/next_prior.hpp>

#include <rtbxegen/lib/openrtb/openrtb.h>

using namespace std;
using namespace rtbxegen;

using folly::dynamic;
using folly::parseJson;
using folly::toJson;

TEST(OpenRTB, GeoTest) {
    dynamic geoObj = dynamic::object
        ("country","USA")
        ("lat",42.355316)
        ("lon",-87.85189)
        ("type",1)
        ("zip","60085");

    Geo geo;
    geo.deserialize(geoObj);

    EXPECT_EQ(geo.zip, geoObj.at("zip").asString().toStdString());
    EXPECT_EQ(geo.country, geoObj.at("country").asString().toStdString());

    dynamic serializedObj = dynamic::object
            ("country","USA")
            ("zip","60085");
    EXPECT_EQ(serializedObj, geo.serialize());
}