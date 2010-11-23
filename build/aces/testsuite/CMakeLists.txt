rosbuild_add_library(aces-testsuite ../src/testsuite.cpp)

rosbuild_add_executable(testsuite ../src/test/testsuite.cpp)
target_link_libraries(testsuite aces-common)
target_link_libraries(testsuite aces-testsuite)
target_link_libraries(testsuite aces-dispatcher)
target_link_libraries(testsuite orocos-taskbrowser-gnulinux)

#target_link_libraries(testsuite aces-webots)
#target_link_libraries(testsuite aces-hubo)
