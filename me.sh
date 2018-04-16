#!/bin/bash

./tft r
./tft R270
./tft 32

hostname=`hostname`
./tft +1 $hostname

. /etc/os-release
#./tft +2 "$PRETTY_NAME" 
IFS=' '
set -- $PRETTY_NAME
./tft +2 "$1 $2 $3" 

#. /etc/armbian-release
#./tft +2 "Armbian "$VERSION

#ipaddr=`sudo ifconfig eth0 |grep "inet addr" |awk {'print $2'} |cut -f2 -d:`
ipaddr=`sudo ifconfig wlan0 |grep "inet addr" |awk {'print $2'} |cut -f2 -d:`
./tft +3 $ipaddr

model=`./tft M`
./tft +4 $model

#raspberry pi
#temp0=`vcgencmd measure_temp`

#orange pi
temp0=`cat /sys/devices/virtual/thermal/thermal_zone0/temp`
./tft +5 "thermal_zone0="$temp0

temp1=`cat /sys/devices/virtual/thermal/thermal_zone1/temp`
./tft +6 "thermal_zone1="$temp1

./tft B0 BLACK
./tft F1 CYAN
./tft F2 WHITE
./tft F3 WHITE
./tft F4 WHITE
./tft F5 WHITE
./tft F6 WHITE
sudo ./tft s

sleep 10
./tft r
./tft B0 BLACK
sudo ./tft s

