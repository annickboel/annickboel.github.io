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
   ((choice=choice-0))
   echo $choice
}

execute_shells() {
  inodes=$(ls $1/*)
  for inode in $inodes
      do
         if [ $inode != "update.sh" ] &&  [ $inode != "php.sh" ] && [ $inode != "mysql.sh" ]
            then
               ./$inode
         fi
      done
}


handle_choice() {
  i=0
  for inode in $inodes
      do
         if [ "$i" = "$choice" ] && [ -d $inode ]
            then
               execute_shells $inode
         fi
         ((i++))
      done
      
}

declare choice
declare inodes

list_directories
choice=$(read_choice)
handle_choice $choice

