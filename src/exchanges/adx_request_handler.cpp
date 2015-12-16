#include "adx_request_handler.h"

namespace rtbxegen {
    AdxRequestHandler::AdxRequestHandler() {

    }

    AdxRequestHandler::~AdxRequestHandler() {

    }

    void AdxRequestHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {

    }

    //this will process the request body
    void AdxRequestHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
        ExchangeRequestHandler::onBody(std::move(body));
    }

    void AdxRequestHandler::onEOM() noexcept {

    }

    void AdxRequestHandler::requestComplete() noexcept {
        delete this;
    }

    void AdxRequestHandler::onError(proxygen::ProxygenError err) noexcept {
        delete this;
    }
}