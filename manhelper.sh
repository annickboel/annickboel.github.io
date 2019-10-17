#!/bin/bash
COMMAND=$1
OPTION=$2
FLAG_NAME=$3

usage() {
  echo ""
  echo "Usage: ./manhelper.sh [COMMAND] <OPTION> [FLAG NAME]"
  echo "OPTION:"
  echo -e "\t--usageflag [FLAG NAME] : Display information for the [FLAG NAME]"
  echo -e "\t--describe : Display description of the command"
}

display_information() {
  echo "display_information $1 $2"
}

display_description() {
  echo "display_description $1"
  man $1 | awk '/DESCRIPTION/,/AUTHOR/' | grep -v AUTHOR 
  exit 
}

invalid_option() {
  echo "Invalid OPTION $1" >&2
  usage
  exit
}

# Control man is installed
is_man_installed() {
   if ! [ -x "$(command -v man)" ]; then
      echo 'Error: man is not installed.' >&2
      exit 1
   fi
}

is_man_installed

# The script has arguments
COUNTER=0
COMMAND=""
while  [ $# -gt 0 ] 
   do
      ARG="$1"
      FLAG_NAME=""
      if [ $COUNTER -eq 0 ]
         then
            COMMAND=$ARG
            COUNTER=`expr $COUNTER + 1`
            shift
         else
            case $ARG in
               --usageflag)
               FLAG_NAME="$2"
               display_information $COMMAND $FLAG_NAME
               shift
               shift
               COUNTER=`expr $COUNTER + 2`
               ;;
               --describe)
               display_description $COMMAND
               shift
               COUNTER=`expr $COUNTER + 1`
               ;;
               *)  # Unknown option
               invalid_option $ARG
            esac
      fi
   done

# The script has no argument

if [ $COUNTER -eq 0 ]
   then
      usage
fi





