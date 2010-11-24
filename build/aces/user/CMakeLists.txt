include_directories("/usr/local/webots/include/controller/c")
link_directories("/usr/local/webots/lib")

rosbuild_add_executable(userProg ../src/test/userProg.cpp)
target_link_libraries(userProg Controller)

target_link_libraries(userProg aces-common)
target_link_libraries(userProg aces-dispatcher)
target_link_libraries(userProg aces-webots)
target_link_libraries(userProg aces-hubo)
target_link_libraries(userProg orocos-taskbrowser-gnulinux)
#needs to be removed
target_link_libraries(userProg aces-testsuite)

#-----------------------------------------------

rosbuild_add_executable(userSM ../src/test/userSM.cpp)
target_link_libraries(userSM Controller)

target_link_libraries(userSM aces-common)
target_link_libraries(userSM aces-dispatcher)
target_link_libraries(userSM aces-webots)
target_link_libraries(userSM aces-hubo)
target_link_libraries(userSM orocos-taskbrowser-gnulinux)
#needs to be removed
target_link_libraries(userSM aces-testsuite)
