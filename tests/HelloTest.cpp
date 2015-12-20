#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <functional>

#include <gtest/gtest.h>
#include <folly/json.h>
#include <folly/io/IOBuf.h>

#include <boost/next_prior.hpp>

using namespace std;

using folly::dynamic;
using folly::parseJson;
using folly::toJson;

TEST(Hello, HelloFirst) {
    int n = 0;
    EXPECT_EQ(0, n);

    //convert unsignint8 array into string
    uint8_t arr[] = {'h','e','l','l','o'};
    std::string s(arr, arr + sizeof(arr) / sizeof(arr[0]));

    EXPECT_EQ(s, "hello");

    const char* sPtr = reinterpret_cast<char*>(&arr[0]);
    s.assign(sPtr);
    EXPECT_EQ(s, "hello");

    //using casting
    uint8_t arr1[] = {'h','e','l','l','o', '\0'};
    std::string o = reinterpret_cast<char*>(arr1);

    EXPECT_EQ(o, "hello");

    std::string t = reinterpret_cast<char*>(arr);
    EXPECT_EQ(t, "hello");
}

TEST(Hello, TestDynamic) {
    dynamic d = dynamic::object();
    d = {1,2,3,4};
    EXPECT_EQ(d.isArray(), true);
    EXPECT_EQ(d.size(), 4);

    //more complex object
    dynamic complexObj = dynamic::object
            ("foo", "bar")
            ("junk", 12)
            ("another", 32.2)
            ("a",
             {
                     dynamic::object("a", "b")
                             ("c", "d"),
                     12.5,
                     "Yo Dawg",
                     { "heh" },
                     nullptr
             }
            )
    ;

    EXPECT_EQ(parseJson(toJson(complexObj)), complexObj);

    //non ascii encoding
    folly::json::serialization_opts opts;
    opts.encode_non_ascii = true;

    // simple tests -- some unicode character is encoded by 2, 3 byte code points
    EXPECT_EQ(folly::json::serialize("\x1f", opts), R"("\u001f")");
    EXPECT_EQ(folly::json::serialize("\xc2\xa2", opts), R"("\u00a2")");
}

TEST(Hello, TestIOBuf) {
    dynamic complexObj = dynamic::object("success", "true");

    //serialize it into fb string data type
    folly::fbstring fbString = folly::toJson(complexObj);

    //create an ioBuf to contain the data
    std::unique_ptr<folly::IOBuf> ioBuf = folly::IOBuf::copyBuffer(fbString.toStdString());

    //get the allocated buffer data
    const uint8_t* bufData = ioBuf->data();
    std::string bufString(bufData, bufData + ioBuf->length());
    EXPECT_EQ(parseJson(bufString), complexObj);

    //another way to create the iobuf
    folly::IOBuf ioBuf1(folly::IOBuf::CopyBufferOp::COPY_BUFFER , fbString.toStdString());
    std::string bufString1(ioBuf1.data(), ioBuf1.data() + ioBuf1.length());

    EXPECT_EQ(parseJson(bufString1), complexObj);

    //TODO: other method to taking ownership goes here
}

struct Number {
    Number(int val) : val_(val) {}

    //const get() function; not updating the class member
    int plus(int num) const {
        return val_ + num;
    }

    int val_;
};


TEST(Hello, StdFunction){
    std::function<int(const Number&, int)> sumF = &Number::plus;
    const Number number(1);
    EXPECT_EQ(sumF(number, 1), 2);

    int n = 1;

    //capture external parameters / variable
    std::function<int(int)> sumF2 = [&](int a){
        return n + a;
    };

    EXPECT_EQ(sumF2(1), 2);

    using std::placeholders::_1;
    std::function<int(int)> sumF3 = std::bind(&Number::plus, &number, _1);
    EXPECT_EQ(sumF3(1), 2);
    EXPECT_EQ(std::bind(&Number::plus, &number, _1)(1), 2);

    std::function<int(int)> sumF4 = std::bind(&Number::plus, number, _1);
    EXPECT_EQ(sumF4(1), 2);
}