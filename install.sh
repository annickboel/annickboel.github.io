#!/bin/bash


usage() {
   echo "Usage: ./install.sh"
}

if [ "$1" = "--help" ]
   then
      usage
fi

list_directories() {
   i=1
   inodes=$(ls)
   array=($(echo $inodes | tr ' ' "\n"))
   nb=${#array[@]}
   ((nb--))

   echo "found $nb folders:"
   for inode in $inodes 
      do
         if [ -d $inode ]
            then 
               echo -e "\t$i - $inode"
               ((i++))
         fi
      done
   echo "enter (1 - $nb) [Default All]:"
}

read_choice() {
   read choice
   return $choice
}
list_directories
choice=$(read_choice)
echo $choice
