SUBDIRS = .

ACLOCAL_AMFLAGS = -I m4

CPPFLAGS += -I$(top_builddir)/.. -I$(top_builddir)/tests/gmock-1.7.0/gtest -I$(top_builddir)/tests/gmock-1.7.0 -I$(top_builddir)/tests/gmock-1.7.0/gtest/include -I$(top_builddir)/tests/gmock-1.7.0/include

check_PROGRAMS = mock_http_server_test
mock_http_server_test_SOURCES = \
	MockHttpServerTest.cpp \
	../TestRequestHandler.cpp

mock_http_server_test_LDADD = \
	../../tests/libtestmain.la \
	-lpthread -lproxygenhttpserver -lfolly -lglog -lgflags

TESTS = mock_http_server_test
