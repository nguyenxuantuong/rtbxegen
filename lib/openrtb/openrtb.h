#pragma once

#include "iostream"

#include <glog/logging.h>
#include <gflags/gflags.h>
#include <folly/Portability.h>

#include <proxygen/httpserver/HTTPServerOptions.h>
#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <proxygen/httpserver/ResponseBuilder.h>
#include <folly/json.h>

#include "boost/optional.hpp"
#include <rtbxegen/lib/common/json.h>

using namespace std;
using namespace proxygen;
using namespace folly;

namespace rtbxegen {
    struct Site : public Serializable {
        ~Site();
        Site() {}

        std::string id;
        std::string page;

        //optional fields
        boost::optional<std::string> name;
        boost::optional<std::string> domain;
        boost::optional<vector<std::string>> cat;

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };

    struct Geo : public Serializable {
        ~Geo();
        Geo() {}

        boost::optional<std::string> country;
        boost::optional<std::string> region;
        boost::optional<std::string> metro;
        boost::optional<std::string> city;
        boost::optional<std::string> zip;

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };

    struct Device : public Serializable {
        ~Device();
        Device() { }

        std::string ip;
        boost::optional<Geo> geo;

        boost::optional<std::string> carrier;
        boost::optional<std::string> language;
        boost::optional<std::string> make;
        boost::optional<std::string> model;
        boost::optional<std::string> os;
        boost::optional<std::string> osv;

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };


    struct Banner : public Serializable {
        ~Banner();
        Banner(){};

        //adx sent multiple size in the banner object; so it need to be vector here
        vector<int> w;
        vector<int> h;
        std::string id;

        boost::optional<int> wmax;
        boost::optional<int> hmax;
        boost::optional<int> wmin;
        boost::optional<int> hmin;
        boost::optional<int> pos;

        folly::dynamic ext = dynamic::object();

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };


    struct Impression : public Serializable {
        ~Impression();
        Impression() {
            banner = Banner();
        }

        std::string id;

        boost::optional<Banner> banner;

        boost::optional<double> bidfloor;
        boost::optional<std::string> bidfloorcur;

        folly::dynamic ext = dynamic::object();;

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };


    struct User : public Serializable {
        ~User();
        User() {}

        std::string id;
        std::string buyeruid;

        boost::optional<std::string> gender;
        boost::optional<std::string> customdata;
        boost::optional<Geo> geo;

        folly::dynamic ext = dynamic::object();

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };

    struct Publisher : public Serializable {
        ~Publisher();
        Publisher(){};

        std::string id;
        boost::optional<std::string> name;
        boost::optional<std::string> domain;
        boost::optional<vector<std::string>> cat;

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };

    struct App : public Serializable {
        ~App();
        App(){}

        std::string id;
        boost::optional<std::string> name;
        boost::optional<std::string> domain;
        boost::optional<vector<std::string>> cat;

        boost::optional<Publisher> publisher;

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };

    struct BidRequest : public Serializable {
        ~BidRequest();
        BidRequest() {
        }

        std::string id;
        std::vector<Impression> imp;

        boost::optional<Site> site;
        boost::optional<App> app;
        boost::optional<Device> device;
        boost::optional<User> user;

        boost::optional<std::vector<string>> cur;
        boost::optional<std::vector<string>> badv;

        folly::dynamic ext = dynamic::object();
        folly::dynamic unparseable = dynamic::object();

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };
}