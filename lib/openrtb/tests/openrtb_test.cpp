#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <functional>
#include <fstream>

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

std::string readFile(std::string fileName) {
    std::ifstream t(fileName);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

TEST(OpenRTB, GeoTest) {
    dynamic geoObj = dynamic::object
        ("country","USA")
        ("lat",42.355316)
        ("lon",-87.85189)
        ("type",1)
        ("zip","60085");

    Geo geo;
    geo.deserialize(geoObj);

    EXPECT_EQ(geo.zip.get(), geoObj.at("zip").asString().toStdString());
    EXPECT_EQ(geo.country.get(), geoObj.at("country").asString().toStdString());

    dynamic serializedObj = dynamic::object
            ("country","USA")
            ("zip","60085");
    EXPECT_EQ(serializedObj, geo.serialize());
}

TEST(OpenRTB, SiteTest) {
    dynamic siteObj = parseJson(readFile("testdata/site.json"));

    Site site;
    site.deserialize(siteObj);

    std::function<void()> checkFunction = [&](){
        EXPECT_EQ(site.id, siteObj.at("id").asString().toStdString());
        EXPECT_EQ(site.domain.get(), siteObj.at("domain").asString().toStdString());
        EXPECT_EQ(site.page, siteObj.at("page").asString().toStdString());

        dynamic catObj = {};
        for(auto elem : site.cat.get()) {
            catObj.push_back(elem);
        }

        EXPECT_EQ(catObj, siteObj.at("cat"));
    };

    checkFunction();

    siteObj = site.serialize();
    checkFunction();
}

TEST(OpenRTB, DeviceTest) {
    dynamic deviceObj = parseJson(readFile("testdata/device.json"));

    Device device;
    device.deserialize(deviceObj);

    std::function<void()> checkFunction = [&](){
        EXPECT_EQ(device.ip, deviceObj.at("ip").asString().toStdString());

        EXPECT_EQ(device.language.get(), deviceObj.at("language").asString().toStdString());
        EXPECT_EQ(device.carrier.get(), deviceObj.at("carrier").asString().toStdString());
        EXPECT_EQ(device.model.get(), deviceObj.at("model").asString().toStdString());
        EXPECT_EQ(device.os.get(), deviceObj.at("os").asString().toStdString());
        EXPECT_EQ(device.osv.get(), deviceObj.at("osv").asString().toStdString());
        EXPECT_EQ(device.make.get(), deviceObj.at("make").asString().toStdString());

        //geo
        EXPECT_EQ(device.geo.get().city, deviceObj.at("geo").at("city").asString().toStdString());
        EXPECT_EQ(device.geo.get().country, deviceObj.at("geo").at("country").asString().toStdString());
        EXPECT_EQ(device.geo.get().metro, deviceObj.at("geo").at("metro").asString().toStdString());
        EXPECT_EQ(device.geo.get().region, deviceObj.at("geo").at("region").asString().toStdString());
        EXPECT_EQ(device.geo.get().zip, deviceObj.at("geo").at("zip").asString().toStdString());
    };

    checkFunction();

    //serialize object
    deviceObj = device.serialize();

    //check again with new serialized device object
    checkFunction();
}

TEST(OpenRTB, BannerTest) {
    dynamic bannerObj = parseJson(readFile("testdata/banner.json"));

    Banner banner;
    banner.deserialize(bannerObj);

    std::function<void()> checkFunction = [&](){
        EXPECT_EQ(banner.w.size(), 1);
        EXPECT_EQ(banner.h.size(), 1);
        EXPECT_EQ(banner.w.at(0), bannerObj.at("w").asInt());
        EXPECT_EQ(banner.h.at(0), bannerObj.at("h").asInt());
    };

    checkFunction();

    bannerObj = banner.serialize();
    checkFunction();

    //check when banner width, height is vector
    bannerObj = dynamic::object
            ("w", {300, 400})
            ("h", {600, 800});

    banner.deserialize(bannerObj);

    checkFunction = [&](){
        EXPECT_EQ(banner.w.size(), 2);
        EXPECT_EQ(banner.h.size(), 2);
        EXPECT_EQ(banner.w.at(0), bannerObj.at("w")[0].asInt());
        EXPECT_EQ(banner.w.at(1), bannerObj.at("w")[1].asInt());
        EXPECT_EQ(banner.h.at(0), bannerObj.at("h")[0].asInt());
        EXPECT_EQ(banner.h.at(1), bannerObj.at("h")[1].asInt());
    };

    checkFunction();

    bannerObj = banner.serialize();
    checkFunction();
}

TEST(OpenRTB, ImpressionTest) {
    dynamic impressionObj = parseJson(readFile("testdata/impression.json"));

    Impression imp;
    imp.deserialize(impressionObj);

    std::function<void()> checkFunction = [&](){
        EXPECT_EQ(imp.id, impressionObj.at("id").asString().toStdString());
        EXPECT_EQ(imp.bidfloor.get(), impressionObj.at("bidfloor").asDouble());
        EXPECT_EQ(imp.banner.get().serialize(), impressionObj.at("banner"));
    };

    checkFunction();

    impressionObj = imp.serialize();
    checkFunction();
}


TEST(OpenRTB, AppTest) {
    dynamic appObj = parseJson(readFile("testdata/app.json"));

    App app;
    app.deserialize(appObj);

    std::function<void()> checkFunction = [&](){
        EXPECT_EQ(app.id, appObj.at("id").asString().toStdString());
        EXPECT_EQ(app.name.get(), appObj.at("name").asString().toStdString());

        dynamic catArr = {};
        for(auto elem : app.cat.get()) {
            catArr.push_back(elem);
        }

        EXPECT_EQ(catArr, appObj.at("cat"));

        EXPECT_EQ(app.publisher.get().id, appObj.at("publisher").at("id").asString().toStdString());
        EXPECT_EQ(app.publisher.get().domain, appObj.at("publisher").at("domain").asString().toStdString());
        EXPECT_EQ(app.publisher.get().name, appObj.at("publisher").at("name").asString().toStdString());
    };

    checkFunction();

    appObj = app.serialize();
    checkFunction();
}

template<typename T>
void compare(vector<T> vect, dynamic obj) {
    dynamic array = {};

    for(auto elem : vect) {
        array.push_back(elem);
    }

    EXPECT_EQ(array, obj);
}

TEST(OpenRTB, BidRequestTest) {
    dynamic breqObj = parseJson(readFile("testdata/breq.banner.json"));

    BidRequest breq;
    breq.deserialize(breqObj);

    std::function<void()> checkFunction = [&](){
        EXPECT_EQ(breq.id, breqObj.at("id").asString().toStdString());
        EXPECT_EQ(breq.device.get().ip, breqObj.at("device").at("ip").asString().toStdString());
        EXPECT_EQ(breq.imp[0].banner.get().w[0], breqObj.at("imp")[0].at("banner").at("w").asInt());
        EXPECT_EQ(breq.imp[0].banner.get().h[0], breqObj.at("imp")[0].at("banner").at("h").asInt());
        EXPECT_EQ(breq.user.get().id, breqObj.at("user").at("id").asString().toStdString());
        EXPECT_EQ(breq.user.get().buyeruid, breqObj.at("user").at("buyeruid").asString().toStdString());
        compare<std::string>(breq.badv.get(), breqObj.at("badv"));
        compare<std::string>(breq.site.get().cat.get(), breqObj.at("site").at("cat"));
    };

    checkFunction();

    breqObj = breq.serialize();
    checkFunction();
}
