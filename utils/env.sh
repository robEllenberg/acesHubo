#!/usr/bin/env bash

ACES_ROOT=$HOME/thesis/aces

source /opt/ros/cturtle/setup.sh
#RTT_COMPONENT_PATH=/home/bob/thesis/aces/build:/opt/ros/cturtle/stacks/orocos_toolchain_ros/rtt/install/lib/orocos
export ROS_PACKAGE_PATH=$ACES_ROOT/build:$ROS_PACKAGE_PATH
export RTT_COMPONENT_PATH=/opt/ros/cturtle/stacks/orocos_toolchain_ros/rtt/install/lib/orocos
export PATH=$ACES_ROOT/build/aces/bin:$PATH

#LD_LIBRARY_PATH=$ACES_ROOT/support/lib:/usr/local/webots/lib/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/usr/local/webots/lib/:$LD_LIBRARY_PATH

export WEBOTS_STDOUT=1
export WEBOTS_STDERR=1
export WEBOTS_STDIN=1

MATLABPATH=$ACES_ROOT/utils:$MATLABPATH
export MATLABPATH
