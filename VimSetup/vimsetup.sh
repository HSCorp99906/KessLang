#!/bin/bash

# VIMSETUP

find ~/.vim | head -n 1 &> vimexi
find ~/.vimrc | head -n 1 &> vimrcexi

VIMEXI=$(cat vimexi)
VIMRCEXI=$(cat vimrcexi)
EXI=$(command -v vim >/dev/null 2>&1 || { echo "__NULL__";})

setup () {
    echo "Setting up highlighting.."
    echo "highlight OUT ctermfg=LightRed" >> ~/.vimrc
    echo "highlight BAK ctermfg=LightRed" >> ~/.vimrc
    echo "highlight INT_KEYWORD ctermfg=yellow" >> ~/.vimrc
}


if [ "$VIMEXI" != "$HOME/.vim" ]
then
    echo "Vim path not found."
    echo "Checking if Vim is installed.."
    sleep 1
    if [ "$EXI" == "__NULL__" ]
    then
        echo "ERROR: Vim is not installed, please install vim. (Terminated)"
        exit
    else
        mkdir ~/.vim
        echo "Setting up .kess extension detection.."
        mv ftdetect ~/.vim
        echo "Setting up syntax.."
        mv syntax ~/.vim
    fi
else
    echo "Setting up .kess extension detection.."
    mv ftdetect ~/.vim
    echo "Setting up syntax.."
    mv syntax ~/.vim
fi

if [ "$VIMRCEXI" != "$HOME/.vimrc" ]
then
    touch ~/.vimrc
    setup
else
    setup
fi

rm vimexi
rm vimrcexi

echo "Done! You may now create a .kess file."
rm vimsetup.sh
