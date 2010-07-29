OPTION( BUILD_WEBOTS "Build Webots" ON )

IF ( BUILD_WEBOTS )

   FILE( GLOB SRCS ../../src/webots.cpp)
   FILE( GLOB HPPS ../../src/webots.hpp)

   INCLUDE_DIRECTORIES( ${WEBOTS_INCLUDE_DIRS} )

   GLOBAL_ADD_COMPONENT( aces-dispatcher ${SRCS} )
   GLOBAL_ADD_INCLUDE( aces ${HPPS})

   COMPONENT_ADD_DEPS( aces-common )
   COMPONENT_ADD_LIBS( aces-common )
   #ADD_SUBDIRECTORY( tests )

   #PROGRAM_ADD_DEPS( wbtest orocos-taskb ) 

ENDIF ( BUILD_WEBOTS )
