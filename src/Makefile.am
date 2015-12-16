SUBDIRS = . exchanges

bin_PROGRAMS = myecho
myecho_LDADD = -lproxygenhttpserver -lfolly -lglog -lgflags
myecho_SOURCES = EchoHandler.cpp EchoServer.cpp
myecho_LDFLAGS = -pthread