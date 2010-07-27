CXX = g++
CP = cp
OCLFLAGS = -DOROCOS_TARGET=gnulinux
INCLUDES = -I../../support/include -I/usr/local/webots/include/controller/c/
LIBS = -L../../support/lib -L/usr/local/webots/lib/
CXXFLAGS = $(INCLUDES) $(LIBS)
DEBUG = -g

hubo: robotisFlex.o svalue.o  hardware.o credentials.o state.o message.o  protocol.o robotis.o 
	$(CXX) $(CXXFLAGS) $(OCLFLAGS) $(DEBUG) ${^} -lorocos-rtt-gnulinux -lorocos-ocl-common-gnulinux -lorocos-taskbrowser-gnulinux -o controller

webots-mini: controller.o goal.o device.o webots.o hardware.o protocol.o state.o dispatcher.o message.o credentials.o wbtest.o taskcfg.o logger.o
	$(CXX) $(CXXFLAGS) $(OCLFLAGS) $(DEBUG) ${^} -lorocos-rtt-gnulinux -lorocos-ocl-common-gnulinux -lorocos-taskbrowser-gnulinux -lController -o minihubo
	$(CP) minihubo ../webots/controllers/minihubo/

webots-hubo: controller.o testsuite.o hardware.o protocol.o state.o svalue.o message.o credentials.o webots.o wbtest.o
	$(CXX) $(CXXFLAGS) $(OCLFLAGS) $(DEBUG) ${^} -lorocos-rtt-gnulinux -lorocos-ocl-common-gnulinux -lorocos-taskbrowser-gnulinux -lController -o hubo
	$(CP) hubo ../webots/controllers/hubo/

test: controller.o testsuite.o hardware.o protocol.o state.o svalue.o message.o credentials.o test.o
	$(CXX) $(CXXFLAGS) $(OCLFLAGS) $(DEBUG) ${^} -lorocos-rtt-gnulinux -lorocos-ocl-common-gnulinux -lorocos-taskbrowser-gnulinux -o test

robotisFlex.o: robotis.l
	flex --header-file=robotis.yy.h --outfile=robotis.yy.c robotis.l
	$(CXX) $(CXXFLAGS) $(OCLFLAGS) $(DEBUG) -c -lfl robotis.yy.c -o robotisFlex.o

clean:
	@-rm test atlasleg controller miniHubo hubo
	@-rm ../webots/controllers/minihubo/minihubo
	@-rm ../webots/controllers/hubo/hubo
	@-rm *.o
	@-rm robotis.yy.c robotis.yy.h
	@-rm orocos.log .tb_history test.io

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $(OCLFLAGS) $(DEBUG) -c ${<} -o ${@} 

