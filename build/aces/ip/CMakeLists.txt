include_directories("/usr/local/webots/include/controller/c")
link_directories("/usr/local/webots/lib")

rosbuild_add_executable(acesIP ../src/test/acesIP.cpp)
target_link_libraries(acesIP Controller)

target_link_libraries(acesIP aces-common)
target_link_libraries(acesIP aces-dispatcher)
target_link_libraries(acesIP aces-webots)
target_link_libraries(acesIP orocos-taskbrowser-gnulinux)

#needs to be removed
target_link_libraries(acesIP aces-testsuite)
