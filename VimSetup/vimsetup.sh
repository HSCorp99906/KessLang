#!/bin/bash

# VIMSETUP

VIMPATH=$(find ~/.vim | head -n 1)
VIMRCPATH=$(find ~/.vimrc | head -n 1)
EXI=$(command -v vim >/dev/null 2>&1 || { echo "__NULL__";})

setup () {
    echo "Setting up highlighting.."
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
    else
        mkdir ~/.vim
    fi
else
    echo "Setting up .kess extension detection.."
    mv ftdetect ~/.vim
    echo "Setting up syntax.."
    mv syntax ~/.vim
fi

if [ "$VIMRCPATH" != "$HOME/.vimrc" ]
then
    touch ~/.vimrc
    setup
else
    setup
fi

echo "Done! You may now create a .kess file."
