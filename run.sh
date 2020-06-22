#!/bin/sh
cd ./build
chmod 777 elevator_monitor

cameraoderfile=`ls /etc/udev/rules.d | grep "10-my-webcam.rules" |wc -l`
if [ $cameraoderfile -eq 0 ];then
sudo cp ../script/10-my-webcam.rules /etc/udev/rules.d
sudo reboot
fi

videonumpast=`ls /dev | grep "video[5-7]" |wc -l`

while true
do
videonum=`ls /dev | grep "video[5-7]" |wc -l`
procnum=`ps -ef|grep "elevator_monitor"|grep -v "grep"|wc -l`
if [ $procnum -eq 0 ] || [ $videonumpast -ne $videonum ]; then
killall -9 elevator_monitor
sudo ./elevator_monitor &
else
echo "already started"   
fi
videonumpast=$videonum
sleep 1
done
