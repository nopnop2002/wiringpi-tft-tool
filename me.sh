#!/bin/bash
#set -x

./tft r
./tft R90
./tft 32

hostname=`hostname`
./tft P1 2
./tft +1 $hostname

. /etc/os-release
IFS=' '
set -- $PRETTY_NAME
./tft P2 2
./tft +2 "$1" 

./tft P3 2
./tft +3 "$2 $3" 

ipaddr=`sudo ifconfig eth0 |grep "inet" |awk {'print $2'} |cut -f2 -d:`
if [ -z ${ipaddr} ]; then
  ipaddr=`sudo ifconfig wlan0 |grep "inet" |awk {'print $2'} |cut -f2 -d:`
fi
if [ -z ${ipaddr} ]; then
  ipaddr=`sudo ifconfig wlan1 |grep "inet" |awk {'print $2'} |cut -f2 -d:`
fi
./tft P4 2
./tft +4 $ipaddr

model=`./tft M`
./tft P5 2
./tft +5 $model

temp0=`vcgencmd measure_temp`
./tft P6 2
./tft +6 $temp0

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

