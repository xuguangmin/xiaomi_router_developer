#!/bin/sh

ip=`ifconfig enp5s0| grep 'netmask' |awk '{print $2}'`
echo $ip
/home/gm/xiaomi_router_developer/NTP_server/ntp-pc/x86-ntp-server $ip & 	#pc ip
touch /hello.txt
/usr/sbin/shutdown 10
