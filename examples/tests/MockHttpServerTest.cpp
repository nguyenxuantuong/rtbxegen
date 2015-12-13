#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <list>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <folly/json.h>
#include <folly/io/IOBuf.h>

#include <proxygen/httpserver/Mocks.h>

#include <boost/next_prior.hpp>

#include "rtbxegen/examples/TestRequestHandler.h"
#include "rtbxegen/examples/TestRequestRecorder.h"

using namespace std;

using folly::dynamic;
using folly::parseJson;
using folly::toJson;

using namespace testing;
using namespace proxygen;

class MockRequestRecorder : public TestRequestRecorder {
public:
    MOCK_METHOD0(recordRequest, void());
    MOCK_METHOD0(getRequestCount, uint64_t());
};

class MockRequestHandlerFixture : public testing::Test {
public:
    void SetUp() override {
        requestHandler = new TestRequestHandler(&stats);

        //response handler is mocked from from request handler
        responseHandler = folly::make_unique<MockResponseHandler>(requestHandler);
    }

    void TearDown() override {
        Mock::VerifyAndClear(&stats);
        Mock::VerifyAndClear(responseHandler.get());
    }

protected:
    TestRequestHandler* requestHandler{nullptr};
    StrictMock<MockRequestRecorder> stats;
    std::unique_ptr<MockResponseHandler> responseHandler;
};

TEST_F(MockRequestHandlerFixture, OnProperRequestSendResponse) {
    //mock methods
    EXPECT_CALL(stats, recordRequest()).WillOnce(Return());
    EXPECT_CALL(stats, getRequestCount()).WillOnce(Return(5));

    HTTPMessage response;
    EXPECT_CALL(*responseHandler, sendHeaders(_)).WillOnce(
            DoAll(SaveArg<0>(&response), Return()));
    EXPECT_CALL(*responseHandler, sendEOM()).WillOnce(Return());

    // Since we know we dont touch request, its ok to pass `nullptr` here.
    requestHandler->onRequest(nullptr);
    requestHandler->onEOM();
    requestHandler->requestComplete();

    EXPECT_EQ("5", response.getHeaders().getSingleOrEmpty("Request-Number"));
    EXPECT_EQ(200, response.getStatusCode());
}