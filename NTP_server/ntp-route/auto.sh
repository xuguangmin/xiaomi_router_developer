#!/bin/sh

cp ntp /mnt/etc/init.d/
cd /mnt/etc/rc.d 
ln -s ../init.d/ntp  S99ntp
echo "127.0.0.1 time.windows.com" >> /mnt/etc/hosts
echo "127.0.0.1 times.windows.com" >> /mnt/etc/hosts
