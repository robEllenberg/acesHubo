INCLUDE( ${PROJ_SOURCE_DIR}/config/DependentOption.cmake )

#
# If we're using gcc, make sure the version is OK.
#
#STRING( REGEX MATCH gcc USING_GCC ${CMAKE_C_COMPILER} )
# IF (  ${CMAKE_C_COMPILER} MATCHES gcc )
#   EXEC_PROGRAM( ${CMAKE_C_COMPILER} ARGS --version OUTPUT_VARIABLE CMAKE_C_COMPILER_VERSION )
#   # Why doesn't this work?
#   #STRING( REGEX MATCHALL "gcc\.*" VERSION_STRING ${CMAKE_C_COMPILER} )
#   IF( CMAKE_C_COMPILER_VERSION MATCHES ".*4\\.[0-9]\\.[0-9]" )
#     MESSAGE("gcc version: ${CMAKE_C_COMPILER_VERSION}")
# #   ELSE(CMAKE_C_COMPILER_VERSION MATCHES ".*4\\.[0-9]\\.[0-9]")
# #     MESSAGE("ERROR: You seem to be using gcc version:")
# #     MESSAGE("${CMAKE_C_COMPILER_VERSION}")
# #     MESSAGE( FATAL_ERROR "ERROR: For gcc, Orocos requires version 4.x")
#   ENDIF(CMAKE_C_COMPILER_VERSION MATCHES ".*4\\.[0-9]\\.[0-9]")
# ENDIF (  ${CMAKE_C_COMPILER} MATCHES gcc )


#
# Check for Doxygen and enable documentation building
#
INCLUDE( ${CMAKE_ROOT}/Modules/FindDoxygen.cmake )
IF ( DOXYGEN )
  MESSAGE( "Found Doxygen -- documentation can be built" )

ELSE ( DOXYGEN )
  MESSAGE( "Doxygen not found -- unable to build documentation" )
ENDIF ( DOXYGEN )

DEPENDENT_OPTION( DOC_GENERATE_API "Build API Documentation" OFF "DOXYGEN" OFF )


#
# An option for tests, to make it easy to turn off all tests
#
OPTION( BUILD_TESTS "Turn me off to disable compilation of all tests" OFF )

