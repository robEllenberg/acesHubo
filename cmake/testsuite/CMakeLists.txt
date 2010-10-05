OPTION( BUILD_TESTSUITE "Build TestSuite" ON )

IF ( BUILD_TESTSUITE )

   FILE( GLOB SRCS ../../src/testsuite.cpp)
   FILE( GLOB HPPS ../../src/testsuite.hpp)

    FILE(GLOB TSSRCS ../../src/uitest.cpp )
    FILE(GLOB TSHPPS ../../src/uitest.hpp )

   #INCLUDE_DIRECTORIES( ${WEBOTS_INCLUDE_DIRS} )

   GLOBAL_ADD_COMPONENT( aces-testsuite ${SRCS} )
   GLOBAL_ADD_INCLUDE( aces ${HPPS})

   COMPONENT_ADD_DEPS( aces-testsuite aces-common )
   #COMPONENT_ADD_LIBS( ${WEBOTS_LIBS} )
    #orocos-ocl-common-gnulinux orocos-rtt-gnulinux orocos-taskbrowser-gnulinux
   #ADD_SUBDIRECTORY( tests )

    ADD_EXECUTABLE( testsuite ${TSSRCS} )
    TARGET_LINK_LIBRARIES( testsuite ${OROCOS_RTT_LIBS} )
    TARGET_LINK_LIBRARIES( testsuite ${OROCOS_OCL_LIBS} )

    ADD_EXECUTABLE( credtest ../../src/test/credeq.cpp )
    TARGET_LINK_LIBRARIES( credtest ${OROCOS_RTT_LIBS} )
    TARGET_LINK_LIBRARIES( credtest ${OROCOS_OCL_LIBS} )
    PROGRAM_ADD_DEPS( credtest aces-common ) 
    PROGRAM_ADD_DEPS( credtest aces-webots ) 

    #TARGET_LINK_LIBRARIES( wbtest ${WEBOTS_LIBS} )
    MESSAGE( STATUS "${OROCOS_OCL_LIBS}")
    MESSAGE( STATUS "${OROCOS_RTT_LIBS}")
    PROGRAM_ADD_DEPS( testsuite aces-dispatcher ) 

ENDIF ( BUILD_TESTSUITE )
