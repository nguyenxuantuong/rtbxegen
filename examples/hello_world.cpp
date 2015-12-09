#include "iostream"

#include <glog/logging.h>
#include <gflags/gflags.h>
#include <folly/Portability.h>
#include <proxygen/httpserver/HTTPServerOptions.h>
#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <proxygen/httpserver/ResponseBuilder.h>

using namespace std;
using namespace proxygen;
using namespace folly;

DEFINE_int32(http_port, 11000, "Port to listen on with HTTP protocol");
DEFINE_string(ip, "*", "IP/Hostname to bind to"); //default bind it to all available network interface
DEFINE_int32(threads, 0, "Number of threads to listen on. Numbers <= 0 "
        "will use the number of cores on this machine.");

DEFINE_bool(allowNameLookUp, true, "Allow host name lookup for HTTP server IP bidding. It's slow operation");

class MockRequestHandler : public proxygen::RequestHandler {
public:
    explicit MockRequestHandler();
    void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;
    void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;
    void requestComplete() noexcept override;
    void onError(proxygen::ProxygenError err) noexcept override;
    void onEOM() noexcept override;
    void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;
private:
    std::unique_ptr<folly::IOBuf> body_;
};

MockRequestHandler::MockRequestHandler(){}

void MockRequestHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {
    LOG(INFO) << "mock request handler receive new request" << endl;
}

//this will process the request body
void MockRequestHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
    //chain is circular; so prependChain will just append to end of the chain; not head of chain
    if(body_){
        body_->prependChain(std::move(body));
    } else {
        body_ = std::move(body);
    }
}

void MockRequestHandler::onEOM() noexcept {
    ResponseBuilder(downstream_)
        .status(200, "OK")
        .header("Content-Type", "application/json")
        .body(std::move(body_))
        .sendWithEOM();
}

void MockRequestHandler::onUpgrade(UpgradeProtocol protocol) noexcept {}

void MockRequestHandler::requestComplete() noexcept {
    delete this;
}

void MockRequestHandler::onError(proxygen::ProxygenError err) noexcept {
    delete this;
}

class MockRequestHandlerFactory : public RequestHandlerFactory {
public:
    void onServerStart(folly::EventBase* evb) noexcept override {}

    void onServerStop() noexcept override {}

    RequestHandler* onRequest(RequestHandler*, HTTPMessage*) noexcept override {
        return new MockRequestHandler();
    }
private:

};


int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    // Initialize Google's logging library.
    google::InitGoogleLogging(argv[0]);

    // Most flags work immediately after updating values.
    FLAGS_logtostderr = 1;

    //if number of thread not being provided; using number of thread equal to number of cores
    if(FLAGS_threads == 0) {
        FLAGS_threads = sysconf(_SC_NPROCESSORS_ONLN);
        CHECK(FLAGS_threads > 0);
    }

    //declare http server options
    HTTPServerOptions serverOptions;
    serverOptions.threads = static_cast<size_t>(FLAGS_threads);
    serverOptions.shutdownOn = {SIGINT, SIGTERM};
    serverOptions.enableContentCompression = true; //gzip content or not
    serverOptions.idleTimeout = std::chrono::milliseconds(60000);

    //attach the handler factory
    serverOptions.handlerFactories = RequestHandlerChain()
            .addThen<MockRequestHandlerFactory>()
            .build();

    //create HTTP server from the HTTP server options
    //HTTPServer constructor is not copiable/clonable; so we have to use std::move
    HTTPServer httpServer(std::move(serverOptions));

    //Create list of IP bidding
    std::vector<HTTPServer::IPConfig> ipConfigs = {
            {folly::SocketAddress(FLAGS_ip, FLAGS_http_port, FLAGS_allowNameLookUp), HTTPServer::Protocol::HTTP }
    };

    httpServer.bind(ipConfigs);

    //using & to capture every variable from outer loop
    std::thread t([&]() -> void {
        LOG(INFO) << "server is started on port: " << FLAGS_http_port << endl;
        httpServer.start();
    });

    //join with main thread
    t.join();

    return 0;
}