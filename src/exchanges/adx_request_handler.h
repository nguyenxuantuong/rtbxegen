#pragma once

#include "exchange_request_handler.h"

using namespace std;
using namespace proxygen;
using namespace folly;

namespace rtbxegen {

    class AdxRequestHandler : public ExchangeRequestHandler {
    public:
        AdxRequestHandler();

        ~AdxRequestHandler();

        static std::string exchangeNameString() {
            return "adx";
        }

        virtual std::string exchangeName() const {
            return exchangeNameString();
        };

        //those requests are for extending standard proxygen handling functions
        virtual void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;
        virtual void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;
        virtual void requestComplete() noexcept override;
        virtual void onError(proxygen::ProxygenError err) noexcept override;
        virtual void onEOM() noexcept override;

    private:

    };

}