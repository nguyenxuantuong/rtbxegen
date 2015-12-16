#include "smaato_request_handler.h"
#include "adx_request_handler.h"

/*
 * Declare some of the default options
 */
DEFINE_int32(http_port, 11000, "Port to listen on with HTTP protocol");
DEFINE_string(ip, "*", "IP/Hostname to bind to"); //default bind it to all available network interface
DEFINE_int32(threads, 0, "Number of threads to listen on. Numbers <= 0 "
        "will use the number of cores on this machine.");

DEFINE_bool(allowNameLookUp, true, "Allow host name lookup for HTTP server IP bidding. It's slow operation");

using namespace rtbxegen;

/*
 * REQUEST HANDLER FACTORY -- which return specific handler based on the specific HTTP Method / Path
 * We use different path for different exchanges
 */
class ExchangeRequestHandlerFactory : public RequestHandlerFactory {
public:
    void onServerStart(folly::EventBase* evb) noexcept override {
    }

    void onServerStop() noexcept override {
    }

    RequestHandler* onRequest(RequestHandler*, HTTPMessage*) noexcept override {
        //pass the instance of request recorder into the real handler
        return new SmaatoRequestHandler();
    }
private:

};

/*
 * Main entry class
 */
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
    serverOptions.contentCompressionLevel = 1; //it has to be from 0->9

    //attach the handler factory
    serverOptions.handlerFactories = RequestHandlerChain()
            .addThen<ExchangeRequestHandlerFactory>()
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