SUBDIRS = .

BUILT_SOURCES = gmock-1.7.0/gtest/src/gtest-all.cc

gmock-1.7.0/gtest/src/gtest-all.cc:
	wget https://googlemock.googlecode.com/files/gmock-1.7.0.zip
	unzip gmock-1.7.0.zip

# libgmockgtest.la is gmock + gtest
check_LTLIBRARIES = libgmockgtest.la
libgmockgtest_la_CPPFLAGS = -Igmock-1.7.0/gtest -Igmock-1.7.0 -Igmock-1.7.0/gtest/include -Igmock-1.7.0/include -lglog

libgmockgtest_la_SOURCES = \
	gmock-1.7.0/gtest/src/gtest-all.cc \
	gmock-1.7.0/src/gmock-all.cc

ACLOCAL_AMFLAGS = -I m4

CPPFLAGS += -Igmock-1.7.0/gtest -Igmock-1.7.0 -Igmock-1.7.0/gtest/include -Igmock-1.7.0/include

check_LTLIBRARIES += libtestmain.la
libtestmain_la_SOURCES = TestMain.cpp
libtestmain_la_LIBADD = libgmockgtest.la

# maintain list of test
TESTS= \
	hello_test

# check to make sure
check_PROGRAMS =

# adding test detail
hello_test_SOURCES = HelloTest.cpp
hello_test_LDADD = libtestmain.la -lpthread -lfolly -lglog -lgflags

# adding test to make check
check_PROGRAMS += $(TESTS)