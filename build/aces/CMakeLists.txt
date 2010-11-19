cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})

rosbuild_add_library(aces-common src/taskcfg.cpp src/word.cpp
                     src/credentials.cpp src/state.cpp
                     src/device.cpp src/protocol.cpp
                     src/hardware.cpp src/controller.cpp src/logger.cpp)
                     
add_definitions(-DTESTSUITE)
#add_definitions(-DROBOTIS)
#add_definitions(-DHUBO)
#add_definitions(-DWEBOTS)

#include_directories("/usr/local/webots/include/controller/c")
#link_directories("/usr/local/webots/lib")

rosbuild_add_library(aces-testsuite src/testsuite.cpp)

#rosbuild_add_library(aces-webots src/webots.cpp)
#target_link_libraries(aces-webots Controller)

#rosbuild_add_library(aces-robotis src/robotis.cpp)

rosbuild_add_library(aces-dispatcher src/dispatcher.cpp)

#rosbuild_add_library(aces-hubo src/hubo/hubo.cpp src/hubo/huboTxPacket.cpp)

#find_package(FLEX)
#FLEX_TARGET(Scanner robotis.l robotis.yy.c)
#rosbuild_add_library(robotis-flex ${FLEX_Scanner_OUTPUTS})

rosbuild_add_executable(testsuite src/test/testsuite.cpp)
target_link_libraries(testsuite aces-common)
target_link_libraries(testsuite aces-testsuite)
#target_link_libraries(testsuite aces-webots)
#target_link_libraries(testsuite aces-hubo)
target_link_libraries(testsuite aces-dispatcher)
target_link_libraries(testsuite orocos-taskbrowser-gnulinux)

#rosbuild_add_executable(wbtest src/test/wbtest.cpp)
#target_link_libraries(wbtest aces-common)
#target_link_libraries(wbtest aces-dispatcher)
#target_link_libraries(wbtest aces-testsuite)
#target_link_libraries(wbtest aces-webots)
#target_link_libraries(wbtest aces-hubo)
#target_link_libraries(wbtest orocos-taskbrowser-gnulinux)

#rosbuild_add_executable(robotis src/test/robotistest.cpp)
#target_link_libraries(robotis aces-common)
#target_link_libraries(robotis aces-dispatcher)
#target_link_libraries(robotis aces-testsuite)
#target_link_libraries(robotis aces-webots)
#target_link_libraries(robotis aces-robotis)
#target_link_libraries(robotis orocos-taskbrowser-gnulinux)

#rosbuild_add_executable(hubovia src/test/hubotest1.cpp)
#target_link_libraries(hubovia aces-common)
#target_link_libraries(hubovia aces-dispatcher)
#target_link_libraries(hubovia aces-testsuite)
#target_link_libraries(hubovia aces-hubo)
#target_link_libraries(hubovia aces-webots)
#target_link_libraries(robotis aces-robotis)
#target_link_libraries(hubovia orocos-taskbrowser-gnulinux)
