#!/bin/bash
# rm /home/rafa/tmp/*  > /dev/null 2>&1 
# cd /home/rafa/ryva
# nohup ./adquirir-tmp.sh /home/rafa/tmp/ > /dev/null 2>&1 &
# sudo -u www-data sudo -u rafa /usr/local/bin/start.sh
sudo -u rafa /usr/local/bin/stop.sh
echo "Content-type: text/html"
echo ""
echo "Sistema autonomo server Finalizado"
