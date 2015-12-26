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

        bool out = json::deserialize<std::string>("ip", ip, jsonObj) &&
               json::deserialize<std::string>("carrier", carrier, jsonObj) &&
               json::deserialize<std::string>("language", language, jsonObj) &&
               json::deserialize<std::string>("make", make, jsonObj) &&
               json::deserialize<std::string>("model", model, jsonObj) &&
               json::deserialize<std::string>("os", os, jsonObj) &&
               json::deserialize<std::string>("osv", osv, jsonObj);

        //only corresponding field exist; then we do deserialize
        if(jsonObj.get_ptr("geo")) {
            geo = Geo();
            out = out && json::deserialize<rtbxegen::Geo>("geo", geo.get(), jsonObj);
        }

        return out;
    }

    //USER
    User::~User() { }

    bool User::deserialize(const folly::dynamic& jsonObj) {
        if(!jsonObj.isObject()) return false;

        bool out = json::deserialize<std::string>("id", id, jsonObj) &&
                   json::deserialize<std::string>("buyeruid", buyeruid, jsonObj) &&
                   json::deserialize<std::string>("gender", gender, jsonObj) &&
                   json::deserialize<std::string>("customdata", customdata, jsonObj);

        //only corresponding field exist; then we do deserialize
        if(jsonObj.get_ptr("geo")) {
            geo = Geo();
            out = out && json::deserialize<rtbxegen::Geo>("geo", geo.get(), jsonObj);
        }

        return out;
    }

    folly::dynamic User::serialize() const {
        folly::dynamic out = folly::dynamic::object();

        json::serialize<std::string>("id", id, out);
        json::serialize<std::string>("buyeruid", buyeruid, out);
        json::serialize<std::string>("gender", gender, out);
        json::serialize<std::string>("customdata", customdata, out);

        //serialize using method overloading; so no need to use generic type <Geo> for instance
        if(geo) json::serialize<rtbxegen::Geo>("geo", geo.get(), out);

        return out;
    }

    //IMPRESSION
    Impression::~Impression() { }

    bool Impression::deserialize(const folly::dynamic& jsonObj) {
        if(!jsonObj.isObject()) return false;

        bool out = json::deserialize<std::string>("id", id, jsonObj) &&
               json::deserialize<std::string>("bidfloorcur", bidfloorcur, jsonObj) &&
               json::deserialize<double>("bidfloor", bidfloor, jsonObj);

        if(jsonObj.get_ptr("banner")) {
            banner = Banner();
            out = out && json::deserialize<rtbxegen::Serializable>("banner", banner.get(), jsonObj);
        }

        return out;
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
        json::serialize<int>("pos", pos, out);

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
               json::deserialize<int>("hmin", hmin, jsonObj) &&
               json::deserialize<int>("pos", pos, jsonObj);

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

    //Publisher
    Publisher::~Publisher() { }

    bool Publisher::deserialize(const folly::dynamic& jsonObj) {
        if(!jsonObj.isObject()) return false;

        return json::deserialize<std::string>("id", id, jsonObj) &&
               json::deserialize<std::string>("name", name, jsonObj) &&
               json::deserialize<std::string>("domain", domain, jsonObj) &&
               json::deserialize<std::string>("cat", cat, jsonObj);
    }

    folly::dynamic Publisher::serialize() const {
        folly::dynamic out = folly::dynamic::object();

        json::serialize<std::string>("id", id, out);
        json::serialize<std::string>("name", name, out);
        json::serialize<std::string>("domain", domain, out);
        json::serialize<std::string>("cat", cat, out);

        return out;
    }

    //APP
    App::~App() { }

    bool App::deserialize(const folly::dynamic& jsonObj) {
        if(!jsonObj.isObject()) return false;

        bool out = json::deserialize<std::string>("id", id, jsonObj) &&
               json::deserialize<std::string>("name", name, jsonObj) &&
               json::deserialize<std::string>("domain", domain, jsonObj) &&
               json::deserialize<std::string>("cat", cat, jsonObj);

        if(jsonObj.get_ptr("publisher")) {
            publisher = Publisher();
            out = out && json::deserialize<Publisher>("publisher", publisher.get(), jsonObj);
        }

        return out;
    }

    folly::dynamic App::serialize() const {
        folly::dynamic out = folly::dynamic::object();

        json::serialize<std::string>("id", id, out);
        json::serialize<std::string>("name", name, out);
        json::serialize<std::string>("domain", domain, out);
        json::serialize<std::string>("cat", cat, out);

        if(publisher){
            json::serialize<Publisher>("publisher", publisher.get(), out);
        }

        return out;
    }

    //BID REQUEST
    BidRequest::~BidRequest() { }

    bool BidRequest::deserialize(const folly::dynamic& jsonObj) {
        if(!jsonObj.isObject()) return false;

        bool out = json::deserialize<std::string>("id", id, jsonObj) &&
                   json::deserialize<std::string>("cur", cur, jsonObj) &&
                   json::deserialize<std::string>("badv", badv, jsonObj) &&
                   json::deserialize<Impression>("imp", imp, jsonObj);

        if(jsonObj.get_ptr("site")) {
            site = Site();
            out = out && json::deserialize<Site>("site", site.get(), jsonObj);
        }

        if(jsonObj.get_ptr("app")) {
            app = App();
            out = out && json::deserialize<App>("app", app.get(), jsonObj);
        }

        if(jsonObj.get_ptr("device")) {
            device = Device();
            out = out && json::deserialize<Device>("device", device.get(), jsonObj);
        }

        if(jsonObj.get_ptr("user")) {
            user = User();
            out = out && json::deserialize<User>("user", user.get(), jsonObj);
        }

        return out;
    }

    folly::dynamic BidRequest::serialize() const {
        folly::dynamic out = folly::dynamic::object();

        json::serialize<std::string>("id", id, out);
        json::serialize<std::string>("cur", cur, out);
        json::serialize<std::string>("badv", badv, out);
        json::serialize<Impression>("imp", imp, out);

        // serialize
        if(site) json::serialize<Site>("site", site.get(), out);
        if(app) json::serialize<App>("app", app.get(), out);
        if(device) json::serialize<Device>("device", device.get(), out);
        if(user) json::serialize<User>("user", user.get(), out);

        return out;
    }
}

