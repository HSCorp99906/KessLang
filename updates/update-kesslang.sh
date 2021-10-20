#!/bin/bash

EXI=$(command -v wget >/dev/null 2>&1 || { echo "__NULL__";})

if [ "$EXI" == "__NULL__" ]
then
    sudo apt-get install wget
fi

rm ../src/Core.cpp
rm ../src/main.cpp

wget --output-document=../src/Core.cpp https://raw.githubusercontent.com/HSCorp99906/KessLang/master/src/Core.cpp
wget --output-document=../src/main.cpp https://raw.githubusercontent.com/HSCorp99906/KessLang/master/src/main.cpp
cd ../src
make
cd ../updates
echo "\nDone!"
