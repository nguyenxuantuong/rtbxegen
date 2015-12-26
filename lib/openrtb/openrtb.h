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
        Site() {

        }

        std::string id;
        std::string page;

        //optinal fields
        boost::optional<std::string> name;
        boost::optional<std::string> domain;
        boost::optional<vector<std::string>> cat;

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };

    struct Geo : public Serializable {
        ~Geo();
        Geo() {

        }

        std::string country;
        std::string region;
        std::string metro;

        std::string city;
        std::string zip;

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };

    struct Device : public Serializable {
        ~Device();
        Device() {
            geo = Geo();
        }

        std::string ip;
        boost::optional<Geo> geo;

        std::string carrier;
        std::string language;
        std::string make;
        std::string model;
        std::string os;
        std::string osv;

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };


    struct Banner : public Serializable {
        ~Banner();
        Banner(){};

        //adx sent multiple size in the banner object; so it need to be vector here
        vector<int> w;
        vector<int> h;
        int wmax = 0;
        int hmax = 0;
        int wmin = 0;
        int hmin = 0;
        std::string id = "";

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
        double bidfloor = 0;
        std::string bidfloorcur;
        folly::dynamic ext = dynamic::object();;

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };


    struct User : public Serializable {
        ~User();
        User() {
            geo = Geo();
        }

        std::string id;
        std::string buyeruid;
        std::string gender;
        std::string customdata;
        boost::optional<Geo> geo;
        folly::dynamic ext = dynamic::object();

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };

    struct App : public Serializable {
        ~App();
        App(){}

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };

    struct BidRequest : public Serializable {
        ~BidRequest();
        BidRequest() {
            app = App();
            site = Site();
            device = Device();
            user = User();
        }

        std::string id;
        std::vector<Impression> imp;

        boost::optional<Site> site;
        boost::optional<App> app;
        boost::optional<Device> device;
        boost::optional<User> user;

        std::vector<string> cur;
        std::vector<string> badv;

        folly::dynamic ext = dynamic::object();
        folly::dynamic unparseable = dynamic::object();

        virtual folly::dynamic serialize() const = 0;
        virtual bool deserialize(const folly::dynamic& ) = 0;
    };
}