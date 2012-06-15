cmake_minimum_required(VERSION 2.8)

project(CONDUCTOR)
find_package(Orocos-RTT) 

include(${OROCOS-RTT_USE_FILE_PATH}/UseOROCOS-RTT.cmake)

add_library(conductor-common SHARED
            taskcfg.cpp
            word.cpp
            credentials.cpp
            state/state.cpp
            device.cpp
            protocol.cpp
            hardware.cpp
            controller.cpp
            logger.cpp
           )
find_package(OROCOS-RTT REQUIRED rtt-scripting ${RTT_HINTS})
target_link_libraries( conductor-common ${OROCOS-RTT_RTT-SCRIPTING_LIBRARY} )

if(NOT INCLUDE_TESTSUITE)
	set(INCLUDE_TESTSUITE "off" CACHE BOOL "Compile the Testsuite?" FORCE)
endif(NOT INCLUDE_TESTSUITE)

if(NOT INCLUDE_HUBO)
	set(INCLUDE_HUBO "off" CACHE BOOL "Compile Hubo Modules?" FORCE)
endif(NOT INCLUDE_HUBO)

if(NOT INCLUDE_WEBOTS)
	set(INCLUDE_WEBOTS "off" CACHE BOOL "Compile Webots Simulator Support?" FORCE)
endif(NOT INCLUDE_WEBOTS)

if(INCLUDE_WEBOTS)
	#Out of standard order because link_directories must be set before
	#the dispatcher is defined as a library
	set(WEBOTS_INCLUDE "/usr/local/webots/include/controller/c" CACHE STRING
        "Webots headers include directory" FORCE)
	set(WEBOTS_LIB "/usr/local/webots/lib" CACHE STRING
        "Webots libraries directory" FORCE)
	include_directories(${WEBOTS_INCLUDE})
	link_directories(${WEBOTS_LIB})
endif(INCLUDE_WEBOTS)

if(NOT INCLUDE_ROBOTIS)
	set(INCLUDE_ROBOTIS "off" CACHE BOOL "Compile Robotis Motor Support (mini-Hubo)?" FORCE)
endif(NOT INCLUDE_ROBOTIS)

if(NOT INCLUDE_OMNIBOT)
	set(INCLUDE_OMNIBOT "off" CACHE BOOL "Compile Mini-Omnibot Support?" FORCE)
endif(NOT INCLUDE_OMNIBOT)

add_library(conductor-dispatcher SHARED dispatcher.cpp)
target_link_libraries(conductor-dispatcher conductor-common)

if(INCLUDE_TESTSUITE)
    add_definitions(-DTESTSUITE)
    add_library(conductor-testsuite SHARED testsuite.cpp)
	target_link_libraries(conductor-testsuite conductor-common)
	target_link_libraries(conductor-dispatcher conductor-testsuite)
	install(TARGETS conductor-testsuite
			LIBRARY DESTINATION lib)
endif(INCLUDE_TESTSUITE)

if(INCLUDE_HUBO)
	if(NOT HUBO_TESTMODE)
		set(HUBO_TESTMODE "off" CACHE BOOL
           "Compile Hubo module for testing? (ON->Text Files, OFF->CAN Card)"
           FORCE)
	endif(NOT HUBO_TESTMODE)
	if(HUBO_TESTMODE)
		add_definitions(-DHUBO_TESTMODE)
	endif(HUBO_TESTMODE)

	add_definitions(-DHUBO)
    add_library(conductor-hubo SHARED hubo/hubo.cpp hubo/huboCanDS.cpp)
	target_link_libraries(conductor-hubo conductor-common)
	target_link_libraries(conductor-dispatcher conductor-hubo)
	install(TARGETS conductor-hubo
			LIBRARY DESTINATION lib)
endif(INCLUDE_HUBO)

if(INCLUDE_WEBOTS)
	add_definitions(-DWEBOTS)
    add_library(conductor-webots SHARED webots.cpp)
	target_link_libraries(conductor-webots conductor-common)
	target_link_libraries(conductor-webots Controller)
	target_link_libraries(conductor-dispatcher conductor-webots)
	install(TARGETS conductor-webots
            LIBRARY DESTINATION lib)
endif(INCLUDE_WEBOTS)

if(INCLUDE_ROBOTIS)
	add_definitions(-DROBOTIS)
	add_subdirectory(flexscanner)
	target_link_libraries(conductor-dispatcher conductor-robotis)
	#install targets for ROBOTIS in flexscanner CMakeLists
endif(INCLUDE_ROBOTIS)

if(INCLUDE_OMNIBOT)
	add_definitions(-DOMNIBOT)
    add_library(conductor-omnibot SHARED omnibot/omnibot.cpp)
	target_link_libraries(conductor-omnibot conductor-common)
	target_link_libraries(conductor-dispatcher conductor-omnibot)
	install(TARGETS conductor-omnibot
            LIBRARY DESTINATION lib)
endif(INCLUDE_OMNIBOT)

link_directories(${OROCOS-RTT_PATH}/lib)
add_executable(conductor-launcher ../src/launcher.cpp)
target_link_libraries(conductor-launcher conductor-common)
target_link_libraries(conductor-launcher conductor-dispatcher)
target_link_libraries(conductor-launcher orocos-ocl-taskbrowser-gnulinux)

install(TARGETS conductor-common conductor-launcher conductor-dispatcher
		RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)

