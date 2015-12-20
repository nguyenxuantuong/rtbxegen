#include "openrtb_request_handler.h"

namespace rtbxegen {
    OpenRtbRequestHandler::OpenRtbRequestHandler() {

    }

    OpenRtbRequestHandler::~OpenRtbRequestHandler() {

    }

    void OpenRtbRequestHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {

    }

    //this will process the request body
    void OpenRtbRequestHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
        ExchangeRequestHandler::onBody(std::move(body));
        //TODO: by default; parse it into the openRTB format
    }

    void OpenRtbRequestHandler::onEOM() noexcept {
        //TODO: check if there is such an default response

    }

    void OpenRtbRequestHandler::requestComplete() noexcept {
        delete this;
    }

    void OpenRtbRequestHandler::onError(proxygen::ProxygenError err) noexcept {
        delete this;
    }
}