#include "openrtb.h"

namespace rtbxegen {
    //GEO
    Geo::~Geo() { }

    folly::dynamic Geo::serialize() const {
        folly::dynamic out = folly::dynamic::object();

        json::serialize<std::string>("country", country, out);
        json::serialize<std::string>("region", region, out);
        json::serialize<std::string>("metro", metro, out);
        json::serialize<std::string>("city", city, out);
        json::serialize<std::string>("zip", zip, out);

        return out;
    }

    bool Geo::deserialize(const folly::dynamic& jsonObj) {
        if(!jsonObj.isObject()) return false;

        return json::deserialize<std::string>("country", country, jsonObj) &&
               json::deserialize<std::string>("region", region, jsonObj) &&
               json::deserialize<std::string>("metro", metro, jsonObj) &&
               json::deserialize<std::string>("city", city, jsonObj) &&
               json::deserialize<std::string>("zip", zip, jsonObj);
    }

    //SITE
    Site::~Site() { }

    folly::dynamic Site::serialize() const {
        folly::dynamic out = folly::dynamic::object();

        json::serialize<std::string>("id", id, out);
        json::serialize<std::string>("name", name, out);
        json::serialize<std::string>("domain", domain, out);
        json::serialize<std::string>("page", page, out);

        json::serialize<std::string>("cat", cat, out);

        return out;
    }

    bool Site::deserialize(const folly::dynamic& jsonObj) {
        if(!jsonObj.isObject()) return false;

        return json::deserialize<std::string>("id", id, jsonObj) &&
               json::deserialize<std::string>("name", name, jsonObj) &&
               json::deserialize<std::string>("domain", domain, jsonObj) &&
               json::deserialize<std::string>("page", page, jsonObj) &&
               json::deserialize<std::string>("cat", cat, jsonObj);
    }

    //DEVICE
    Device::~Device() { }

    folly::dynamic Device::serialize() const {
        folly::dynamic out = folly::dynamic::object();

        json::serialize<std::string>("ip", ip, out);
        json::serialize<std::string>("carrier", carrier, out);
        json::serialize<std::string>("language", language, out);
        json::serialize<std::string>("make", make, out);
        json::serialize<std::string>("model", model, out);
        json::serialize<std::string>("os", os, out);
        json::serialize<std::string>("osv", osv, out);

        //serialize using method overloading; so no need to use generic type <Geo> for instance
        if(geo) json::serialize<rtbxegen::Geo>("geo", geo.get(), out);

        return out;
    }

    bool Device::deserialize(const folly::dynamic& jsonObj) {
        if(!jsonObj.isObject()) return false;

        return json::deserialize<std::string>("ip", ip, jsonObj) &&
               json::deserialize<std::string>("carrier", carrier, jsonObj) &&
               json::deserialize<std::string>("language", language, jsonObj) &&
               json::deserialize<std::string>("make", make, jsonObj) &&
               json::deserialize<std::string>("model", model, jsonObj) &&
               json::deserialize<std::string>("os", os, jsonObj) &&
               json::deserialize<std::string>("osv", osv, jsonObj) &&
               json::deserialize<rtbxegen::Geo>("geo", geo.get(), jsonObj);
    }

    //USER
    User::~User() { }

    bool User::deserialize(const folly::dynamic& jsonObj) {
        return true;
    }

    folly::dynamic User::serialize() const {
        return folly::dynamic::object();
    }

    //IMPRESSION
    Impression::~Impression() { }

    bool Impression::deserialize(const folly::dynamic& jsonObj) {
        if(!jsonObj.isObject()) return false;

        return json::deserialize<std::string>("id", id, jsonObj) &&
               json::deserialize<std::string>("bidfloorcur", bidfloorcur, jsonObj) &&
               json::deserialize<double>("bidfloor", bidfloor, jsonObj) &&
               json::deserialize<rtbxegen::Serializable>("banner", banner.get(), jsonObj);
    }

    folly::dynamic Impression::serialize() const {
        folly::dynamic out = folly::dynamic::object();

        json::serialize<std::string>("id", id, out);
        json::serialize<std::string>("bidfloorcur", bidfloorcur, out);
        json::serialize<double>("bidfloor", bidfloor, out);
        if(banner) json::serialize("banner", banner.get(), out);

        return out;
    }

    //BANNER
    Banner::~Banner() { }

    folly::dynamic Banner::serialize() const {
        folly::dynamic out = folly::dynamic::object();

        json::serialize<std::string>("id", id, out);
        json::serialize<int>("wmax", wmax, out);
        json::serialize<int>("hmax", hmax, out);
        json::serialize<int>("hmin", hmin, out);
        json::serialize<int>("wmin", wmin, out);

        //width and height need to be handled slightly different
        if(w.size() > 1) {
            //this will overload vector<int> function instead
            json::serialize<int>("w", w, out);
        } else if(w.size() == 1) {
            json::serialize<int>("w", w.at(0), out);
        }

        if(h.size() > 1) {
            json::serialize<int>("h", h, out);
        } else if(w.size() == 1) {
            json::serialize<int>("h", h.at(0), out);
        }

        return out;
    }

    bool Banner::deserialize(const folly::dynamic& jsonObj) {
        if(!jsonObj.isObject()) return false;

        bool out = json::deserialize<std::string>("id", id, jsonObj) &&
               json::deserialize<int>("wmax", wmax, jsonObj) &&
               json::deserialize<int>("hmax", hmax, jsonObj) &&
               json::deserialize<int>("wmin", wmin, jsonObj) &&
               json::deserialize<int>("hmin", hmin, jsonObj);

        //width and height need to be handle differently
        dynamic wDynamic = jsonObj.at("w");
        dynamic hDynamic = jsonObj.at("h");

        //width
        if(wDynamic.isArray()) {
            out = out && json::deserialize<int>("w", w, jsonObj);
        } else if(wDynamic.isInt()){
            w.push_back(wDynamic.asInt());
        } else return false;

        //height
        if(hDynamic.isArray()) {
            out = out && json::deserialize<int>("h", h, jsonObj);
        } else if(hDynamic.isInt()){
            h.push_back(hDynamic.asInt());
        } else return false;

        return out;
    }

    //APP
    App::~App() { }

    bool App::deserialize(const folly::dynamic& jsonObj) {
        return true;
    }

    folly::dynamic App::serialize() const {
        return folly::dynamic::object();
    }

    //BID REQUEST
    BidRequest::~BidRequest() { }

    bool BidRequest::deserialize(const folly::dynamic& jsonObj) {
        return true;
    }

    folly::dynamic BidRequest::serialize() const {
        return folly::dynamic::object();
    }
}

