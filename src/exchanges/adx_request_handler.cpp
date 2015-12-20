#include "adx_request_handler.h"

#include "folly/json.h"

namespace rtbxegen {
    AdxRequestHandler::AdxRequestHandler() {

    }

    AdxRequestHandler::~AdxRequestHandler() {

    }

    void AdxRequestHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {
        ExchangeRequestHandler::onRequest(std::move(headers));
    }

    //this will process the request body
    void AdxRequestHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
        ExchangeRequestHandler::onBody(std::move(body));
    }

    bool isNeededToHandle (const ::BidRequest& gbr) {
        if (gbr.has_is_ping() && gbr.is_ping())
            return true ;

        if (gbr.has_video())
            return false ;

        if (gbr.has_mobile())
        {
            const auto& mob = gbr.mobile();
            if ((mob.has_is_app() && mob.is_app() && !mob.has_app_id()) ||
                !(gbr.has_url() || gbr.has_anonymous_id()))
                return false;
        }

        return true;
    }

    void AdxRequestHandler::onEOM() noexcept {
        // if Content-Type is not application/octet-stream; return 400 bad request
        std::string contentType = httpMessage->getHeaders().rawGet("Content-Type");
        if(contentType.compare("application/octet-stream") != 0) {
            this->quickResponse(400, "Bad Request", "header is not octet-stream");
            return;
        }

        //try to decode the message
        ::BidRequest gbr;
        if (!gbr.ParseFromString (body_->moveToFbString().toStdString()))
        {
            quickResponse(400, "Bad Request", "couldn't decode bid request message");
            return;
        }

        if(!isNeededToHandle(gbr)){
            quickResponse(204, "No Content", "");
            return;
        }

        LOG(INFO) << "gbr debug string: " << gbr.DebugString() << endl;

        //parse other information over here
        this->quickResponse(200, "OK");
    }

    void AdxRequestHandler::requestComplete() noexcept {
        delete this;
    }

    void AdxRequestHandler::onError(proxygen::ProxygenError err) noexcept {
        delete this;
    }
}