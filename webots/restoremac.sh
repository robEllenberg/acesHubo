#!/usr/bin/env bash

sudo ifconfig wlan0 down
sudo ifconfig wlan0 hw ether 00:13:02:1c:dc:01 
sudo ifconfig wlan0 up
sudo dhclient wlan0
