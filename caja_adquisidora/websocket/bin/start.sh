#!/bin/bash

rm /home/rafa/tmp/*  > /dev/null 2>&1 
cd /home/rafa/ryva
nohup ./adquirir-tmp.sh /home/rafa/tmp/ > /dev/null 2>&1 &

echo Iniciado

