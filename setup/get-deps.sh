#!/bin/bash

if [ "$EUID" -ne 0 ]
then
    echo "Need to run as root."
else
    sudo apt install python3.8
    apt-get update
    apt-get install g++
    apt-get install gcc
    apt-get install make
    apt-get install vim
fi
