OPTION( BUILD_COMMON "Build Common" ON )

IF ( BUILD_COMMON )

   SET(PREFIX ../../src/)
   SET(NODES credentials;goal;message;taskcfg;word;controller;device;hardware;protocol;state)
   SET(srcs "")
   SET(hpps "")

   FOREACH( el ${NODES})
      SET(srcs "${PREFIX}${el}.cpp;${srcs}")
      SET(hpps "${PREFIX}${el}.hpp;${hpps}")
   ENDFOREACH(el)
   #MESSAGE ( STATUS "${srcs}")
   #MESSAGE ( STATUS "${hpps}")
   
   FILE( GLOB SRCS ${srcs})
   FILE( GLOB HPPS ${hpps})

   GLOBAL_ADD_COMPONENT( aces-common ${SRCS} )
   GLOBAL_ADD_INCLUDE( aces ${HPPS})

   #COMPONENT_ADD_DEPS( orocos-taskbrowser orocos-ocl-common )

   #ADD_SUBDIRECTORY( tests )

ENDIF ( BUILD_COMMON )
