#include "smaato_request_handler.h"

namespace rtbxegen {
    SmaatoRequestHandler::SmaatoRequestHandler() {

    }

    SmaatoRequestHandler::~SmaatoRequestHandler() {

    }

    void SmaatoRequestHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {

    }

    //this will process the request body
    void SmaatoRequestHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
        OpenRtbRequestHandler::onBody(std::move(body));
        //TODO: other parser overhere
    }

    void SmaatoRequestHandler::onEOM() noexcept {
        std::unique_ptr<folly::IOBuf> bodyBuf;

        dynamic complexObj = dynamic::object("success", "true");

        //serialize it into fb string data type
        folly::fbstring fbString = folly::toJson(complexObj);

        bodyBuf = folly::IOBuf::copyBuffer(fbString.toStdString());

        //just return directly body as the response
        ResponseBuilder(downstream_)
                .status(200, "OK")
                .header("Content-Type", "application/json")
                .body(std::move(bodyBuf))
                .sendWithEOM();
    }

    void SmaatoRequestHandler::requestComplete() noexcept {
        delete this;
    }

    void SmaatoRequestHandler::onError(proxygen::ProxygenError err) noexcept {
        delete this;
    }
}