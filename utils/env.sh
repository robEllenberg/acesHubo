#!/usr/bin/env bash

LD_LIBRARY_PATH=/home/bob/thesis/support/lib:/usr/local/webots/lib/:$LD_LIBRARY_PATH
RTT_COMPONENT_PATH=/opt/ros/cturtle/stacks/orocos_toolchain_ros/rtt/install/lib/orocos
export RTT_COMPONENT_PATH
export WEBOTS_STDOUT=1
export WEBOTS_STDERR=1
export WEBOTS_STDIN=1
export LD_LIBRARY_PATH

