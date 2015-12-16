#include "exchange_request_handler.h"

namespace rtbxegen {
    ExchangeRequestHandler::ExchangeRequestHandler() {

    }

    ExchangeRequestHandler::~ExchangeRequestHandler() {

    }

    void ExchangeRequestHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {
        //TODO: might want to log the request -- sending to graphite, etc...
    }

    //this will process the request body
    void ExchangeRequestHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
        if (body_) {
            body_->prependChain(std::move(body));
        } else {
            body_ = std::move(body);
        }
    }

    void ExchangeRequestHandler::onEOM() noexcept {
        //TODO: send 204 here
        ResponseBuilder(downstream_)
                .status(204, "No Content")
                .sendWithEOM();
    }

    void ExchangeRequestHandler::onUpgrade(UpgradeProtocol protocol) noexcept { }

    void ExchangeRequestHandler::requestComplete() noexcept {
        delete this;
    }

    void ExchangeRequestHandler::onError(proxygen::ProxygenError err) noexcept {
        delete this;
    }
}