OPTION( BUILD_WEBOTS "Build Webots" ON )

IF ( BUILD_WEBOTS )

   FILE( GLOB SRCS ../../src/webots.cpp)
   FILE( GLOB HPPS ../../src/webots.hpp)

    FILE(GLOB WBSRCS ../../src/wbtest.cpp )
    FILE(GLOB WBHPPS ../../src/wbtest.hpp )

   INCLUDE_DIRECTORIES( ${WEBOTS_INCLUDE_DIRS} )

   GLOBAL_ADD_COMPONENT( aces-webots ${SRCS} )
   GLOBAL_ADD_INCLUDE( aces ${HPPS})

   COMPONENT_ADD_DEPS( aces-webots aces-common )
   COMPONENT_ADD_LIBS( ${WEBOTS_LIBS} )
    #orocos-ocl-common-gnulinux orocos-rtt-gnulinux orocos-taskbrowser-gnulinux
   #ADD_SUBDIRECTORY( tests )

    ADD_EXECUTABLE( wbtest ${WBSRCS} )
    TARGET_LINK_LIBRARIES( wbtest ${OROCOS_RTT_LIBS} )
    TARGET_LINK_LIBRARIES( wbtest ${OROCOS_OCL_LIBS} )
    TARGET_LINK_LIBRARIES( wbtest ${WEBOTS_LIBS} )
    MESSAGE( STATUS "${OROCOS_OCL_LIBS}")
    MESSAGE( STATUS "${OROCOS_RTT_LIBS}")
    PROGRAM_ADD_DEPS( wbtest aces-dispatcher ) 

ENDIF ( BUILD_WEBOTS )
