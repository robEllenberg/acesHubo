CXX = g++
CP = cp
OCLFLAGS = -DOROCOS_TARGET=gnulinux
INCLUDES = -I/home/bob/thesis/support/include -I/usr/local/webots/include/controller/c/
LIBS = -L/home/bob/thesis/support/lib -L/usr/local/webots/lib/
CXXFLAGS = $(INCLUDES) $(LIBS)
DEBUG = -g

all: robotisFlex.o pvalue.o dispatcher.o hardware.o credentials.o parameter.o message.o  protocol.o robotis.o 
	$(CXX) $(CXXFLAGS) $(OCLFLAGS) $(DEBUG) ${^} -lorocos-rtt-gnulinux -lorocos-ocl-common-gnulinux -lorocos-taskbrowser-gnulinux -o controller

webots: controller.o testsuite.o hardware.o protocol.o parameter.o pvalue.o message.o credentials.o dispatcher.o webots.o wbtest.o
	$(CXX) $(CXXFLAGS) $(OCLFLAGS) $(DEBUG) ${^} -lorocos-rtt-gnulinux -lorocos-ocl-common-gnulinux -lorocos-taskbrowser-gnulinux -lController -o minihubo
#	$(CXX) $(CXXFLAGS) $(OCLFLAGS) $(DEBUG) ${^} -lorocos-rtt-gnulinux -lorocos-ocl-common-gnulinux -lorocos-taskbrowser-gnulinux -o minihubo
	$(CP) minihubo /home/bob/thesis/aces/webots/webots-proj/controllers/minihubo/

test: controller.o testsuite.o hardware.o protocol.o parameter.o pvalue.o message.o credentials.o dispatcher.o test.o
	$(CXX) $(CXXFLAGS) $(OCLFLAGS) $(DEBUG) ${^} -lorocos-rtt-gnulinux -lorocos-ocl-common-gnulinux -lorocos-taskbrowser-gnulinux -o test

robotisFlex.o: robotis.l
	flex --header-file=robotis.yy.h --outfile=robotis.yy.c robotis.l
	$(CXX) $(CXXFLAGS) $(OCLFLAGS) $(DEBUG) -c -lfl robotis.yy.c -o robotisFlex.o

clean:
	@-rm test atlasleg controller miniHubo
	@-rm *.o
	@-rm robotis.yy.c robotis.yy.h
	@-rm orocos.log .tb_history test.io

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $(OCLFLAGS) $(DEBUG) -c ${<} -o ${@} 

