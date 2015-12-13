#include "iostream"

#include <glog/logging.h>
#include <gflags/gflags.h>
#include <folly/Portability.h>
#include <proxygen/httpserver/HTTPServerOptions.h>
#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <proxygen/httpserver/ResponseBuilder.h>
#include <folly/json.h>

#include "TestRequestRecorder.h"

using namespace std;
using namespace proxygen;
using namespace folly;

using folly::dynamic;
using folly::parseJson;
using folly::toJson;

/*
 * Mock Request Handler -- which handle specific HTTP request
 */
class TestRequestHandler : public proxygen::RequestHandler {
public:
    explicit TestRequestHandler(TestRequestRecorder *);
    void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;
    void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;
    void requestComplete() noexcept override;
    void onError(proxygen::ProxygenError err) noexcept override;
    void onEOM() noexcept override;
    void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;
private:
    std::unique_ptr<folly::IOBuf> body_;
    TestRequestRecorder * const requestRecorder_;
};