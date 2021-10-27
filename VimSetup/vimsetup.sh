#!/bin/bash

# VIMSETUP

find ~/.vim | head -n 1 &> vimexi
find ~/.vimrc | head -n 1 &> vimrcexi

VIMEXI=$(cat vimexi)
VIMRCEXI=$(cat vimrcexi)
EXI=$(command -v vim >/dev/null 2>&1 || { echo "__NULL__";})

setup () {
    echo "Setting up highlighting.."
    echo "syntax on" >> ~/.vimrc
    echo "highlight OUT ctermfg=red" >> ~/.vimrc
    echo "highlight BAK ctermfg=red" >> ~/.vimrc
    echo "highlight INT_KEYWORD ctermfg=yellow" >> ~/.vimrc
    echo "highlight STRING ctermfg=green" >> ~/.vimrc
    echo "highlight COMMENT ctermfg=blue" >> ~/.vimrc
    echo "highlight INTEGER ctermfg=magenta" >> ~/.vimrc
    echo "highlight FILE_READ_OUT ctermfg=darkmagenta" >> ~/.vimrc
    echo "highlight c_start ctermfg=magenta" >> ~/.vimrc
    echo "highlight c_end ctermfg=magenta" >> ~/.vimrc
    echo "highlight IF_STATEMENT ctermfg=yellow" >> ~/.vimrc
    echo "highlight VARDUMP ctermfg=darkmagenta" >> ~/.vimrc
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
