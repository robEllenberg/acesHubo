#!/usr/bin/env bash

sudo ifconfig eth0 down
sudo ifconfig eth0 hw ether 00:80:48:BA:d1:30
sudo ifconfig eth0 up
sudo dhclient eth0
