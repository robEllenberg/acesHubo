#!/usr/bin/env bash

ACES_ROOT=$HOME/aces

source /opt/ros/diamondback/setup.bash
#RTT_COMPONENT_PATH=/home/bob/thesis/aces/build:/opt/ros/diamondback/stacks/orocos_toolchain_ros/rtt/install/lib/orocos
export ROS_PACKAGE_PATH=$ACES_ROOT/build:$ROS_PACKAGE_PATH
export RTT_COMPONENT_PATH=/opt/ros/diamondback/stacks/orocos_toolchain_ros/rtt/install/lib/orocos

export UTILS=$ACES_ROOT/utils
export ACES_PATH=$ACES_ROOT/build/aces/bin:$UTILS
export PATH=$ACES_PATH:$PATH

#Webots library directories
LD_LIBRARY_PATH=$ACES_ROOT/support/lib:/usr/local/webots/lib/:$LD_LIBRARY_PATH
#Add additional library paths for diamondback, since ROS can't find them:
LD_LIBRARY_PATH=/opt/ros/diamondback/stacks/orocos_toolchain_ros/rtt/install/lib/orocos/gnulinux/plugins:$LD_LIBRARY_PATH
#LD_LIBRARY_PATH=/opt/ros/diamondback/stacks/orocos_toolchain_ros/ocl/install/lib/orocos:$LD_LIBRARY_PATH

export WEBOTS_STDOUT=1
export WEBOTS_STDERR=1
export WEBOTS_STDIN=1

MATLABPATH=$ACES_ROOT/utils:$ACES_ROOT/utils/matlabIO:$MATLABPATH
export MATLABPATH

#stty -F /dev/ttyUSB0 raw
#stty -F /dev/ttyUSB0 1000000
#stty -F /dev/ttyUSB1 raw
#stty -F /dev/ttyUSB1 1000000

