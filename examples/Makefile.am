SUBDIRS = . tests

# TODO: HACK -- TOBE REMOVE
AM_CXXFLAGS = -std=c++11
AM_CPPFLAGS = -I/home/vagrant/data/proxygen

bin_PROGRAMS = mock_http_server
mock_http_server_LDADD = -lproxygenhttpserver -lfolly -lglog -lgflags
mock_http_server_SOURCES = \
    TestHttpServer.cpp \
    TestRequestHandler.cpp

mock_http_server_LDFLAGS = -pthread