AM_CXXFLAGS = -std=c++11
AM_CPPFLAGS = -I/home/vagrant/data/proxygen

bin_PROGRAMS = hello_world
hello_world_LDADD = -lproxygenhttpserver -lfolly -lglog -lgflags
hello_world_SOURCES = hello_world.cpp
hello_world_LDFLAGS = -pthread