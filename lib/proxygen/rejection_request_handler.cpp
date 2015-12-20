#include "rejection_request_handler.h"

namespace rtbxegen {
    RejectionRequestHandler::RejectionRequestHandler() {

    }

    RejectionRequestHandler::~RejectionRequestHandler() {

    }

    void RejectionRequestHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {
        //TODO: might want to log the request -- sending to graphite, etc...
    }

    //this will process the request body
    void RejectionRequestHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
        if (body_) {
            body_->prependChain(std::move(body));
        } else {
            body_ = std::move(body);
        }
    }

    void RejectionRequestHandler::onEOM() noexcept {
        ResponseBuilder(downstream_)
                .status(404, "Not Found")
                .sendWithEOM();
    }

    void RejectionRequestHandler::onUpgrade(UpgradeProtocol protocol) noexcept { }

    void RejectionRequestHandler::requestComplete() noexcept {
        delete this;
    }

    void RejectionRequestHandler::onError(proxygen::ProxygenError err) noexcept {
        delete this;
    }
}