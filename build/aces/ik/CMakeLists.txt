rosbuild_add_library(aces-mini-ik ../src/ik/miniForwardKinematics.c
                                  ../src/ik/miniIKSolve_initialize.c
                                  ../src/ik/miniJacobian.c
                                  ../src/ik/rdivide.c
                                  ../src/ik/rtGetNaN.c
                                  ../src/ik/svd.c
                                  ../src/ik/max.c
                                  ../src/ik/miniIKSolve.c
                                  ../src/ik/miniIKSolve_terminate.c
                                  ../src/ik/norm.c
                                  ../src/ik/rtGetInf.c
                                  ../src/ik/rt_nonfinite.c)

#rosbuild_add_executable(robotis src/test/robotistest.cpp)
#target_link_libraries(robotis aces-common)
#target_link_libraries(robotis aces-dispatcher)
#target_link_libraries(robotis aces-testsuite)
#target_link_libraries(robotis aces-webots)
#target_link_libraries(robotis aces-robotis)
#target_link_libraries(robotis orocos-taskbrowser-gnulinux)

