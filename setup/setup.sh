#!/bin/bash

# SETUP.

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
    rm get-deps.sh
    sleep 1
    echo "Changing directory to ../"
    cd ../
    sleep 2
    echo "Deleting setup folder.."
    sleep 1
    cho "Create a .kess file and run: klcc filename.kess"
    rm -rf setup/
fi
