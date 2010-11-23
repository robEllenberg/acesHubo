include_directories("/usr/local/webots/include/controller/c")
link_directories("/usr/local/webots/lib")

rosbuild_add_library(aces-webots ../src/webots.cpp)
target_link_libraries(aces-webots Controller)

rosbuild_add_executable(wbtest ../src/test/wbtest.cpp)
target_link_libraries(wbtest aces-common)
target_link_libraries(wbtest aces-dispatcher)
target_link_libraries(wbtest aces-webots)
target_link_libraries(wbtest aces-hubo)
target_link_libraries(wbtest orocos-taskbrowser-gnulinux)

#needs to be removed
target_link_libraries(wbtest aces-testsuite)
