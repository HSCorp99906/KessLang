#!/bin/bash

VIMPATH=$(find ~/.vim | head -n 1)
VIMRCPATH=$(find ~/.vimrc | head -n 1)
EXI=$(command -v vim >/dev/null 2>&1 || { echo "__NULL__";})

setup_vimrc () {
    echo "highlight OUT ctermfg=LightRed" >> ~/.vimrc
    echo "highlight BAK ctermfg=LightRed" >> ~/.vimrc
}


if [ "$VIMPATH" != "$HOME/.vim" ]
then
    echo "Vim path not found."
    echo "Checking if vim is installed.."
    sleep 1
    if [ "$EXI" == "__NULL__" ]
    then
        echo "ERROR: Vim is not installed, please install vim. (Terminated)"
        exit
    fi
else
    mv ftdetect ~/.vim
    mv syntax ~/.vim
fi

if [ "$VIMRCPATH" != "$HOME/.vimrc" ]
then
    touch ~/.vimrc
    setup
else
    setup
fi
