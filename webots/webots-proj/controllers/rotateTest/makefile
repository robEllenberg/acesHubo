CXX = g++
CC = gcc
#OCLFLAGS = -DOROCOS_TARGET=gnulinux
#INCLUDES = -I/home/bob/thesis/support/include
#LIBS = -L/home/bob/thesis/support/lib 
INCLUDES = -I/usr/local/webots/include/controller/c/
LIBS = -L/usr/local/webots/lib/
CXXFLAGS = $(INCLUDES) $(LIBS)
CFLAGS = $(INCLUDES) $(LIBS)
#BASE = /home/bob/thesis/support/orocos-rtt-1.10.2/src

#Controller file names: libController.so  libCppController.so

shared:
	$(CC) $(CFLAGS) rotate2.c -lController -o rotateTest

clean:
	-rm bobshoap2
