ACLOCAL_AMFLAGS = -I m4

CPPFLAGS += -I$(top_builddir)/.. -I$(top_builddir)/tests/gmock-1.7.0/gtest -I$(top_builddir)/tests/gmock-1.7.0 -I$(top_builddir)/tests/gmock-1.7.0/gtest/include -I$(top_builddir)/tests/gmock-1.7.0/include

check_PROGRAMS = openrtb_test
openrtb_test_SOURCES = \
	openrtb_test.cpp

openrtb_test_LDADD = \
    ../libopenrtb_proxygen.la \
	../../../tests/libtestmain.la \
	-lpthread -lproxygenhttpserver -lfolly -lglog -lgflags

TESTS = openrtb_test
