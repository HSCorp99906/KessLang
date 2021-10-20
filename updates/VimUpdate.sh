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
echo "syntax on" >> ~/.vimrc
echo "highlight OUT ctermfg=LightRed" >> ~/.vimrc
echo "highlight BAK ctermfg=LightRed" >> ~/.vimrc
echo "highlight INT_KEYWORD ctermfg=yellow" >> ~/.vimrc

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
    > ~/.vim/syntax/kess.vim
    echo "syntax match OUT /out/" >> ~/.vim/syntax/kess.vim
    echo "syntax match BAK /bak/" >> ~/.vim/syntax/kess.vim
    echo "syntax match INT_KEYWORD /int/" >> ~/.vim/syntax/kess.vim
    echo "Done you are now up to date!"
    echo "Deleting VimUpdate.sh..."
    rm VimUpdate.sh
fi
