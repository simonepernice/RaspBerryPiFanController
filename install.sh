#!/bin/sh

echo -e "#modified by homealarm to set the battery test gpio low at boot to avoid battery discharge and circuit damage\ngpio=14=op,dl\n(cat /boot/config.txt)" > /boot/config.txt

mkdir -p /etc/homealarm
cp config/*.cfg /etc/homealarm

mkdir -p /var/log/homealarm

cp bin/arm/homealarm /usr/bin/

cp service/homealarm.* /etc/systemd/system
chmod 644 /etc/systemd/system/homealarm.*

systemctl daemon-reload 

sudo systemctl enable homealarm.timer

sudo systemctl start homealarm.service


