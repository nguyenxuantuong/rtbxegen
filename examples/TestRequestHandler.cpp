#include "TestRequestHandler.h"

TestRequestHandler::TestRequestHandler(TestRequestRecorder * requestRecorder): requestRecorder_(requestRecorder){}

void TestRequestHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {
    //record request
    requestRecorder_->recordRequest();
}

//this will process the request body
void TestRequestHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
    //chain is circular; so prependChain will just append to end of the chain; not head of chain
    if (body_) {
        body_->prependChain(std::move(body));
    } else {
        body_ = std::move(body);
    }
}

void TestRequestHandler::onEOM() noexcept {
    //let log the body first
    if(body_){
        LOG(INFO) << body_->data() << endl;
    }

    std::unique_ptr<folly::IOBuf> bodyBuf;

    if(body_){
        bodyBuf = folly::IOBuf::copyBuffer(body_->data(), body_->length());
    } else {
        dynamic complexObj = dynamic::object("success", "true");

        //serialize it into fb string data type
        folly::fbstring fbString = folly::toJson(complexObj);

        bodyBuf = folly::IOBuf::copyBuffer(fbString.toStdString());
    }

    //just return directly body as the response
    ResponseBuilder(downstream_)
            .status(200, "OK")
            .header("Request-Number", folly::to<std::string>(requestRecorder_->getRequestCount()))
            .body(std::move(body_))
            .sendWithEOM();
}

void TestRequestHandler::onUpgrade(UpgradeProtocol protocol) noexcept {}

void TestRequestHandler::requestComplete() noexcept {
    delete this;
}

void TestRequestHandler::onError(proxygen::ProxygenError err) noexcept {
    delete this;
}
