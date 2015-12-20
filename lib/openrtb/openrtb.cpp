#include "openrtb.h"

namespace rtbxegen {
    //GEO
    Geo::~Geo() { }

    folly::dynamic Geo::serialize() const {
        folly::dynamic out = dynamic::object();

        JsonHelper::serializeHelper("country", country, out);
        JsonHelper::serializeHelper("region", region, out);
        JsonHelper::serializeHelper("metro", metro, out);
        JsonHelper::serializeHelper("city", city, out);
        JsonHelper::serializeHelper("zip", zip, out);

        return out;
    }

    bool Geo::deserialize(const folly::dynamic& jsonObj) {
        if(!jsonObj.isObject()) return false;

        return JsonHelper::deserializeHelper("country", country, jsonObj) &&
        JsonHelper::deserializeHelper("region", region, jsonObj) &&
        JsonHelper::deserializeHelper("metro", metro, jsonObj) &&
        JsonHelper::deserializeHelper("city", city, jsonObj) &&
        JsonHelper::deserializeHelper("zip", zip, jsonObj);
    }

    //SITE
    Site::~Site() { }

    bool Site::deserialize(const folly::dynamic& jsonObj) {
        if(!jsonObj.isObject()) return false;

        return JsonHelper::deserializeHelper("id", id, jsonObj) &&
               JsonHelper::deserializeHelper("name", name, jsonObj) &&
               JsonHelper::deserializeHelper("domain", domain, jsonObj);
    }

    folly::dynamic Site::serialize() const {
        folly::dynamic out = dynamic::object();

        JsonHelper::serializeHelper("id", id, out);
        JsonHelper::serializeHelper("name", name, out);
        JsonHelper::serializeHelper("domain", domain, out);

        return out;
    }

    //DEVICE
    Device::~Device() { }

    bool Device::deserialize(const folly::dynamic& jsonObj) {
        if(!jsonObj.isObject()) return false;

        return JsonHelper::deserializeHelper("ip", ip, jsonObj) &&
               JsonHelper::deserializeHelper("carrier", carrier, jsonObj) &&
               JsonHelper::deserializeHelper("language", language, jsonObj) &&
               JsonHelper::deserializeHelper("make", make, jsonObj) &&
               JsonHelper::deserializeHelper("model", model, jsonObj) &&
               JsonHelper::deserializeHelper("os", os, jsonObj) &&
               JsonHelper::deserializeHelper("osv", osv, jsonObj) &&
               JsonHelper::deserializeHelper("geo", geo.get(), jsonObj);
    }

    folly::dynamic Device::serialize() const {
        folly::dynamic out = dynamic::object();

        JsonHelper::serializeHelper("ip", ip, out);
        JsonHelper::serializeHelper("carrier", carrier, out);
        JsonHelper::serializeHelper("language", language, out);
        JsonHelper::serializeHelper("make", make, out);
        JsonHelper::serializeHelper("model", model, out);
        JsonHelper::serializeHelper("os", os, out);
        JsonHelper::serializeHelper("osv", osv, out);

        if(geo) JsonHelper::serializeHelper("geo", geo.get(), out);

        return out;
    }

    User::~User() { }

    bool User::deserialize(const folly::dynamic& jsonObj) {
        return true;
    }

    folly::dynamic User::serialize() const {
        return dynamic::object();
    }

    Impression::~Impression() { }

    bool Impression::deserialize(const folly::dynamic& jsonObj) {
        return true;
    }

    folly::dynamic Impression::serialize() const {
        return dynamic::object();
    }

    Banner::~Banner() { }

    bool Banner::deserialize(const folly::dynamic& jsonObj) {
        return true;
    }

    folly::dynamic Banner::serialize() const {
        return dynamic::object();
    }

    BidRequest::~BidRequest() { }

    bool BidRequest::deserialize(const folly::dynamic& jsonObj) {
        return true;
    }

    folly::dynamic BidRequest::serialize() const {
        return dynamic::object();
    }
}

