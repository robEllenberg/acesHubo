CXX = g++
OCLFLAGS = -DOROCOS_TARGET=gnulinux
INCLUDES = -I/home/bob/thesis/support/include
LIBS = -L/home/bob/thesis/support/lib 
CXXFLAGS = $(INCLUDES) $(LIBS)
BASE = /home/bob/thesis/support/orocos-rtt-1.10.2/src

shared:
	$(CXX) $(CXXFLAGS) $(OCLFLAGS) ocl_hello.cpp -lorocos-rtt-gnulinux -o oclHello

unshared:
	$(CXX) $(INCLUDES) $(OCLFLAGS) ocl_hello.cpp $(BASE)/TaskContext.cpp $(BASE)/Activity.cpp -o oclHello

clean:
	-rm oclHello
  #include <rtt/TaskContext.hpp>
  #include <rtt/Activity.hpp>
