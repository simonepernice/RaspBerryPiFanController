#!/bin/sh
echo "Run this script with sudo"

cp binary/fancontroller /usr/bin

cp config/fancontroller.cfg /etc/

cp config/fanconcroller.service /etc/systemd/system
chmod 640 /etc/systemd/system/fancontroller.service
systemctl daemon-reload 
systemctl enable fancontroller.service
systemctl start fancontroller.service

