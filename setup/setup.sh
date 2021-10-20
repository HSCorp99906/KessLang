#!/bin/bash

if [ "$EUID" -ne 0 ]
then
    echo "Please run program as root."
else
    echo "Setting up.."
    sleep 1
    cd ../src
    make
    cd ../setup
    mv klcc /bin/
    chmod +x /bin/klcc
    echo "Removing setup junk.."
    sleep 1
    echo "You may now delete the setup folder."
    echo "Create a .kess file and run: klcc filename.kess"
    rm setup.sh
fi
