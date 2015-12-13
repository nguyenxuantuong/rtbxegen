#pragma once

/*
 * It is for recording number of request, etc...
 */
class TestRequestRecorder {
public:
    virtual ~TestRequestRecorder() {
    }

    // NOTE: We make the following methods `virtual` so that we can
    // mock them using Gmock for our C++ unit-tests. EchoStats
    // is an external dependency to handler and we should be
    // able to mock it.

    virtual void recordRequest() {
        ++reqCount_;
    }

    virtual uint64_t getRequestCount() {
        return reqCount_;
    }

private:
    uint64_t reqCount_{0};
};