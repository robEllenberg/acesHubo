#!/usr/bin/env bash

sudo ifconfig wlan0 down
sudo ifconfig wlan0 hw ether 00:16:36:2f:e7:b2 
sudo ifconfig wlan0 up
sudo dhclient wlan0
