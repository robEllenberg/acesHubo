OPTION( BUILD_DISPATCHER "Build Dispatcher" ON )

IF ( BUILD_DISPATCHER )

    FILE( GLOB SRCS ../../src/dispatcher.cpp)
    FILE( GLOB HPPS ../../src/dispatcher.hpp)

    GLOBAL_ADD_COMPONENT( aces-dispatcher ${SRCS} )
    GLOBAL_ADD_INCLUDE( aces ${HPPS})
    COMPONENT_ADD_DEPS( aces-dispatcher aces-common )

    IF( BUILD_WEBOTS )
        INCLUDE_DIRECTORIES( ${WEBOTS_INCLUDE_DIRS} )
        COMPONENT_ADD_DEPS( aces-dispatcher aces-webots )
        ADD_DEFINITIONS( -DWEBOTS )
    ENDIF(BUILD_WEBOTS)

    IF( BUILD_TESTSUITE )
        COMPONENT_ADD_DEPS( aces-dispatcher aces-testsuite )
        ADD_DEFINITIONS(-DTESTSUITE)
    ENDIF( BUILD_TESTSUITE )

    IF( BUILD_ROBOTIS )
        COMPONENT_ADD_DEPS( aces-dispatcher aces-robotis )
        ADD_DEFINITIONS(-DROBOTIS)
    ENDIF( BUILD_ROBOTIS )

    #COMPONENT_ADD_LIBS( aces-common )
    #ADD_SUBDIRECTORY( tests )

    #PROGRAM_ADD_DEPS( wbtest orocos-taskb ) 

ENDIF ( BUILD_DISPATCHER )
