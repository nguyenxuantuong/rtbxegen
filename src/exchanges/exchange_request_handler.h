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

using namespace std;
using namespace proxygen;
using namespace folly;


namespace rtbxegen {

class ExchangeRequestHandler : public proxygen::RequestHandler {
public:
    ExchangeRequestHandler();

    ~ExchangeRequestHandler();

    //this pure virtual function will make the exchange request handler become the abstract class
    //derive class need to make sure follow the same function signature; like std::string exchangeName() const
    virtual std::string exchangeName() const = 0;

    //those requests are for extending standard proxygen handling functions
    virtual void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept;
    virtual void onBody(std::unique_ptr<folly::IOBuf> body) noexcept;
    virtual void requestComplete() noexcept;
    virtual void onError(proxygen::ProxygenError err) noexcept;
    virtual void onEOM() noexcept;
    virtual void onUpgrade(proxygen::UpgradeProtocol proto) noexcept;

protected:
    std::unique_ptr<folly::IOBuf> body_; //keeping http payload
};

}