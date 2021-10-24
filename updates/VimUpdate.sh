#!/bin/bash

echo "Will clear vimrc for update."
read -p "Do you want a backup of your vimrc file? [y/n]: " YN

path=$(find ~/.vimrc)

if [ "$path" != "$HOME/.vimrc" ]
then
    echo "You do not have a vimrc file."
    echo "Please run vimsetup.sh which is located in: $HOME/KessLang/VimSetup/vimsetup.sh"
    exit
fi


if [ "$YN" == "y" ]
then
    backup=$(find ~/backup)
    backup150=$(find ~/backup150)

    if [ "$backup" == "$HOME/backup" ]
    then
        if [ "$backup150" == "$HOME/backup150" ]
        then
            echo "Cannot create backup file, backup150 and backup already exist in $HOME."
            echo "Terminated."
            exit
        else
            mkdir ~/backup150
            cp ~/.vimrc ~/backup150
        fi
    else
        mkdir ~/backup
        cp ~/.vimrc ~/backup
    fi
fi

> ~/.vimrc
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

vimfile=$(find ~/.vim | head -n 1)

if [ "$vimfile" != "$HOME/.vim" ]
then
    ftdetctfile=$(find ../VimSetup/ftdetect | head -n 1)
    if [ "$ftdetctfile" == "../VimSetup/ftdetect" ]
    then
        echo "Please run vimsetup.sh which is located in: $HOME/KessLang/VimSetup/vimsetup.sh before running this again."
        exit
    else
        echo "Something went wrong.. Please contact teaqllabs@gmail.com for assistance.."
        exit
    fi
else
    rm ~/.vim/syntax
    mkdir ~/.vim/syntax
    mv syntax.vim ~/.vim/syntax/kess.vim
    echo "Done! You are now up to date!"
    echo "Deleting VimUpdate.sh..."
    rm VimUpdate.sh
fi
