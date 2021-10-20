#!/bin/bash

if [ "$EUID" -ne 0]
then
    echo "Need to run as root."
else
    apt-get update
    apt-get install g++
    apt-get install make
    apt-get install vim
