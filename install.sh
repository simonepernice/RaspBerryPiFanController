#!/bin/sh

cp fancontroller/bin/Release/fancontroller /usr/bin

cp config/fancontroller.cfg /etc/

cp config/fanconcroller.service /etc/systemd/system
chmod 640 /etc/systemd/system/fancontroller.service
systemctl daemon-reload 
sudo systemctl enable fancontroller.service
sudo systemctl start fancontroller.service

