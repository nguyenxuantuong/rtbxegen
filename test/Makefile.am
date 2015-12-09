SUBDIRS = .

ACLOCAL_AMFLAGS = -I m4

CPPFLAGS += -Igtest-1.7.0/include -lpthread

# maintain list of test
TESTS= \
	hello_test

# check to make sure
check_LTLIBRARIES = libgtestmain.la libgtest.la
check_PROGRAMS =

libgtestmain_la_CPPFLAGS = -Igtest-1.7.0 -Igtest-1.7.0/src
libgtestmain_la_SOURCES = gtest-1.7.0/src/gtest-all.cc gtest-1.7.0/src/gtest_main.cc

libgtest_la_CPPFLAGS = -Igtest-1.7.0 -Igtest-1.7.0/src
libgtest_la_SOURCES = gtest-1.7.0/src/gtest-all.cc

# adding test detail
hello_test_SOURCES = hello_test.cpp
hello_test_LDADD = libgtestmain.la

# adding test to make check
check_PROGRAMS += $(TESTS)