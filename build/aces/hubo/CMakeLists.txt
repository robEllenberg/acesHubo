
rosbuild_add_library(aces-hubo ../src/hubo/hubo.cpp
                               ../src/hubo/huboTxPacket.cpp)

rosbuild_add_executable(hubovia ../src/test/hubotest1.cpp)
target_link_libraries(hubovia aces-common)
target_link_libraries(hubovia aces-dispatcher)
target_link_libraries(hubovia aces-testsuite)
target_link_libraries(hubovia aces-hubo)
#target_link_libraries(hubovia aces-webots)
#target_link_libraries(robotis aces-robotis)
target_link_libraries(hubovia orocos-taskbrowser-gnulinux)
