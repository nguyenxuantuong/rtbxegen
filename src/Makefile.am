AM_CXXFLAGS = -std=c++11
AM_CPPFLAGS = -I/home/vagrant/data/proxygen

bin_PROGRAMS = myecho
myecho_LDADD = -lproxygenhttpserver -lfolly -lglog -lgflags
myecho_SOURCES = EchoHandler.cpp EchoServer.cpp
myecho_LDFLAGS = -pthread