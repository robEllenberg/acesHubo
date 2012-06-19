#!/usr/bin/env bash

ACES_ROOT=$HOME/workspace/hubo-proj/aces
#ACES_ROOT=$PWD
#ROS_DIR=/opt/ros/diamondback
ROS_DIR=/opt/ros/diamondback

source ${ROS_DIR}/setup.sh
#RTT_COMPONENT_PATH=/home/bob/thesis/aces/build:/opt/ros/cturtle/stacks/orocos_toolchain_ros/rtt/install/lib/orocos
export ROS_PACKAGE_PATH=$ACES_ROOT/build:$ACES_ROOT:$ROS_PACKAGE_PATH
export RTT_COMPONENT_PATH=${ROS_DIR}/stacks/orocos_toolchain_ros/rtt/install/lib/orocos
export OCL_PATH=${ROS_DIR}/stacks/orocos_toolchain_ros/ocl

export UTILS=$ACES_ROOT/utils
export ACES_PATH=$ACES_ROOT/build/aces/bin:$UTILS
export PATH=$ACES_PATH:$PATH

#LD_LIBRARY_PATH=$ACES_ROOT/support/lib:/usr/local/webots/lib/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/usr/local/webots/lib/:${ROS_DIR}/stacks/orocos_toolchain_ros/rtt/install/lib:${OCL_PATH}/install/lib:${RTT_COMPONENT_PATH}/gnulinux/plugins:$LD_LIBRARY_PATH

export WEBOTS_STDOUT=1
export WEBOTS_STDERR=1
export WEBOTS_STDIN=1

MATLABPATH=$ACES_ROOT/utils:$ACES_ROOT/utils/matlabIO:$MATLABPATH
export MATLABPATH

#stty -F /dev/ttyUSB0 raw
#stty -F /dev/ttyUSB0 1000000
#stty -F /dev/ttyUSB1 raw
#stty -F /dev/ttyUSB1 1000000

