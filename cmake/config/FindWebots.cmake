
#INCLUDE (${PROJ_SOURCE_DIR}/config/FindPkgConfig.cmake)
#INCLUDE (${PROJ_SOURCE_DIR}/config/component_rules.cmake)

    MESSAGE( STATUS "Detecting Webots" )
    MESSAGE( "Looking for Webots in: ${WEBOTS_INSTALL}")
#    SET(ENV{PKG_CONFIG_PATH} "${WEBOTS_INSTALL}/lib"}

    #PKGCONFIG( "webots" WEBOTS_FOUND WEBOTS_INCLUDE_DIRS WEBOTS_DEFINES WEBOTS_LINK_DIRS WEBOTS_LIBS )

   SET(WEBOTS_LIBS Controller)
   SET(WEBOTS_DEFINES webots/robot.h webots/servo.h)

   SET(SUFFIX lib)
   FOREACH(lib ${WEBOTS_LIBS})
       FIND_LIBRARY(ltemp ${lib} "${WEBOTS_INSTALL}/lib")
                    #ENV WEBOTS_INSTALL )
       GET_FILENAME_COMPONENT(ltemp ${ltemp} PATH)
       SET(WEBOTS_LINK_DIRS ${WEBOTS_LINK_DIRS} ${ltemp})
   ENDFOREACH(lib)

   FOREACH(def ${WEBOTS_DEFINES})
       FIND_FILE(dtemp ${def} "${WEBOTS_INSTALL}/include/controller/c")
                    #ENV WEBOTS_INSTALL PATH_SUFFIXES include/controller/c)
       GET_FILENAME_COMPONENT(dtemp ${dtemp} PATH)
       SET(WEBOTS_INCLUDE_DIRS ${WEBOTS_INCLUDE_DIRS} ${dtemp})
   ENDFOREACH(def)

   FIND_PATH(TEMP Controller /usr/local/webots/lib)
    SET(WEBOTS_FOUND True)
    IF( WEBOTS_FOUND )
        MESSAGE("   Includes in: ${WEBOTS_INCLUDE_DIRS}")
        MESSAGE("   Libraries in: ${WEBOTS_LINK_DIRS}")
        MESSAGE("   Libraries: ${WEBOTS_LIBS}")
        MESSAGE("   Defines: ${WEBOTS_DEFINES}")

	INCLUDE_DIRECTORIES( ${WEBOTS_INCLUDE_DIRS} )
	LINK_DIRECTORIES( ${WEBOTS_LINK_DIRS} )
	OROCOS_PKGCONFIG_REQUIRES("webots")

    ENDIF ( WEBOTS_FOUND )

#/usr/local/webots/include/controller/c
#ELSE  ( CMAKE_PKGCONFIG_EXECUTABLE )
#    MESSAGE( FATAL_ERROR "Can't find Webots without pkgconfig !")
#ENDIF ( CMAKE_PKGCONFIG_EXECUTABLE )
