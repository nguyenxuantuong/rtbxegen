SUBDIRS = . tests

bin_PROGRAMS = mock_http_server
mock_http_server_LDADD = -lproxygenhttpserver -lfolly -lglog -lgflags
mock_http_server_SOURCES = \
    TestHttpServer.cpp \
    TestRequestHandler.cpp

mock_http_server_LDFLAGS = -pthread