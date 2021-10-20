#!/bin/bash

# SETUP.

EXI=$(command -v make >/dev/null 2>&1 || { echo "__NULL__";})
EXI_GPP=$(command -v g++ >/dev/null 2>&1 || { echo "__NULL__";})

if [ "$EXI"  == "__NULL__" || "$EXI_GPP" == "__NULL__" ]
then
    echo "Cannot start due to missing deps. (Terminated)"
    exit
fi

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
    echo "You may now delete the setup folder and proceed to setup vim."
    echo "Create a .kess file and run: klcc filename.kess"
    rm setup.sh
fi
