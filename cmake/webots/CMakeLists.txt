OPTION( BUILD_WEBOTS "Build Webots" ON )

IF ( BUILD_WEBOTS )

   FILE( GLOB SRCS ../../src/webots.cpp)
   FILE( GLOB HPPS ../../src/webots.hpp)

   INCLUDE_DIRECTORIES( ${WEBOTS_INCLUDE_DIRS} )

   GLOBAL_ADD_COMPONENT( aces-webots ${SRCS} )
   GLOBAL_ADD_INCLUDE( aces ${HPPS})

   #FIND_LIBRARY(RTT_LIB_PATH orocos-rtt-gnulinux)
   #FIND_LIBRARY(OCL_LIB_PATH orocos-ocl-common-gnulinux)
   #FIND_LIBRARY(WEBOTS_LIB_PATH Controller)
   COMPONENT_ADD_DEPS( aces-common )
   COMPONENT_ADD_LIBS( aces-common )
    #orocos-ocl-common-gnulinux orocos-rtt-gnulinux orocos-taskbrowser-gnulinux
   #ADD_SUBDIRECTORY( tests )

ENDIF ( BUILD_WEBOTS )
