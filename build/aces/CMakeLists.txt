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
#rosbuild_find_ros_package(rtt)
#find_package(Orocos-RTT HINTS ${rtt_PACKAGE_PATH}/install)
#rosbuild_include(rtt_ros_integration GenerateRTTtypekit) 

rosbuild_find_ros_package(rtt )
find_package(Orocos-RTT HINTS ${rtt_PACKAGE_PATH}/install )
include(${OROCOS-RTT_USE_FILE_PATH}/UseOROCOS-RTT.cmake)

include_directories("/usr/local/webots/include/controller/c")
link_directories("/usr/local/webots/lib")

rosbuild_add_library(aces-common src/taskcfg.cpp src/word.cpp
                     src/credentials.cpp src/state/state.cpp
                     src/device.cpp src/protocol.cpp
                     src/hardware.cpp src/controller.cpp src/logger.cpp)
#rosbuild_add_compile_flags(aces-common -pg)
#rosbuild_add_link_flags(aces-common -pg)

#add_subdirectory(robotis)
#add_subdirectory(ik)
add_subdirectory(flexscanner)
                    
add_definitions(-DTESTSUITE)
add_definitions(-DHUBO)
#add_definitions(-DROBOTIS)
#add_definitions(-DWEBOTS)
#add_definitions(-DOMNIBOT)

rosbuild_add_library(aces-dispatcher src/dispatcher.cpp)
target_link_libraries(aces-dispatcher aces-testsuite)
#target_link_libraries(aces-dispatcher aces-robotis)
#target_link_libraries(aces-dispatcher aces-webots)
#rosbuild_add_compile_flags(aces-dispatcher -pg)
#rosbuild_add_link_flags(aces-dispatcher -pg)

rosbuild_add_library(aces-testsuite ../src/testsuite.cpp)
#rosbuild_add_compile_flags(aces-testsuite -pg)
#rosbuild_add_link_flags(aces-testsuite -pg)

rosbuild_add_library(aces-hubo ../src/hubo/hubo.cpp
#                               ../src/hubo/huboVia.cpp
                               ../src/hubo/huboCanDS.cpp)
#                               ../src/hubo/huboTxPacket.cpp)
#rosbuild_add_compile_flags(aces-hubo -pg)
#rosbuild_add_link_flags(aces-hubo -pg)

#rosbuild_add_library(aces-webots ../src/webots.cpp)
#target_link_libraries(aces-webots Controller)

#rosbuild_add_library(minihubo ../src/minihubo.cpp)
#target_link_libraries(minihubo aces-common minihubo-adc)

#rosbuild_add_library(omnibot ../src/omnibot/omnibot.cpp)
#target_link_libraries(omnibot aces-common)

rosbuild_add_executable(aces-launcher ../src/launcher.cpp)
#rosbuild_add_compile_flags(aces-launcher -pg)
#rosbuild_add_link_flags(aces-launcher -pg)

target_link_libraries(aces-launcher aces-common)
target_link_libraries(aces-launcher aces-dispatcher)
#target_link_libraries(aces-launcher aces-testsuite)
target_link_libraries(aces-launcher aces-hubo)
#target_link_libraries(aces-launcher aces-webots)
#target_link_libraries(aces-launcher aces-robotis)
#target_link_libraries(aces-launcher minihubo)
#target_link_libraries(aces-launcher omnibot)
target_link_libraries(aces-launcher orocos-taskbrowser-gnulinux)
#rosbuild_add_compile_flags(aces-launcher -pg)
#rosbuild_add_link_flags(aces-launcher -pg)
