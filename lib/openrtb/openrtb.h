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

using namespace std;
using namespace proxygen;
using namespace folly;

namespace rtbxegen {
    class Serializable
    {
    public:
        Serializable(){}
        virtual ~Serializable(){}

        virtual folly::dynamic serialize() const = 0;
        virtual bool deserialize(const folly::dynamic& ) = 0;
    };

    struct JsonHelper {
        static void serializeHelper(std::string fieldName, const std::string structField, folly::dynamic& out) {
            if(!structField.empty()){
                out[fieldName] = structField;
            }
        }

        static void serializeHelper(std::string fieldName, const int& structField, folly::dynamic& out) {
            out[fieldName] = structField;
        };

        static void serializeHelper(std::string fieldName, const Serializable& structField, folly::dynamic& out) {
            out[fieldName] = structField.serialize();
        };

        static bool deserializeHelper(std::string fieldName, std::string& structField, const folly::dynamic& jsonObj) {
            if(auto* jsonField = jsonObj.get_ptr(fieldName)){
                if(jsonField->isString()) {
                    structField = jsonField->asString().toStdString();
                } else return false;
            }

            return true;
        };

        static bool deserializeHelper(std::string fieldName, Serializable& structField, const folly::dynamic& jsonObj) {
            if(auto* jsonField = jsonObj.get_ptr(fieldName)){
                return structField.deserialize(jsonObj);
            }

            return true;
        };
    };

    struct Site : public Serializable {
        ~Site();

        std::string id;
        std::string name;
        std::string domain;

        virtual folly::dynamic serialize() const;
        virtual bool deserialize(const folly::dynamic& );
    };

    struct Geo : public Serializable {
        ~Geo();

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

        vector<int> w;
        vector<int> h;
        int wmax;
        int hmax;
        int wmin;
        int hmin;
        std::string id;

        folly::dynamic ext;

        virtual folly::dynamic serialize() const = 0;
        virtual bool deserialize(const folly::dynamic& ) = 0;
    };


    struct Impression : public Serializable {
        ~Impression();
        std::string id;
        boost::optional<Banner> banner;
        double bidfloor;
        std::string bidfloorcur;
        folly::dynamic ext;

        virtual folly::dynamic serialize() const = 0;
        virtual bool deserialize(const folly::dynamic& ) = 0;
    };


    struct User : public Serializable {
        ~User();

        std::string id;
        std::string buyeruid;
        std::string gender;
        std::string customdata;
        boost::optional<Geo> geo;
        folly::dynamic ext;

        virtual folly::dynamic serialize() const = 0;
        virtual bool deserialize(const folly::dynamic& ) = 0;
    };

    struct App : public Serializable {
        ~App(){};

        virtual folly::dynamic serialize() const = 0;
        virtual bool deserialize(const folly::dynamic& ) = 0;
    };

    struct BidRequest : public Serializable {
        ~BidRequest();
        std::string id;
        std::vector<Impression> imp;

        boost::optional<Site> site;
        boost::optional<App> app;
        boost::optional<Device> device;
        boost::optional<User> user;

        std::vector<string> cur;
        std::vector<string> badv;

        folly::dynamic ext;
        folly::dynamic unparseable;

        virtual folly::dynamic serialize() const = 0;
        virtual bool deserialize(const folly::dynamic& ) = 0;
    };
}