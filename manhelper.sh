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

find_next_section_name() {

   #Sections lines start with UPPERCASE
   sections=`man $1 | grep '^[A-Z]'`

   # Remove line fields before the searched field.
   # Take next_field 
   command="echo \$sections |  awk ' { sub(/^.* *$2/, \"\"); print \$1}'"

   # Execute the command
   # Store result in global variable
   retval=`eval $command`
}


find_next_flag_name() {
   echo "display_next_flag_name $1 $2"
   
   #Sections lines start with  
   flags=`man $1 | grep  '$2'`
   echo $flags

}

display_man_section() {
   section=$2

   # Find the name of the next section in manual
   find_next_section_name $1 $2 
   next_section=$retval

   #
   command="man $1 | awk  '/$section/,/$next_section/' | grep -v '^[A-Z]'"
   eval $command

}

display_flagname_information() {
  if [ -z $2 ]
     then
        echo "FLAGNAME is mandatory in --usageflag option"
        usage
     else
        # Find the name of the next section in manual
        find_next_flag_name $1 -v
        next_flag=$retval
        echo $next_flag


  fi
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
            if [ $# -eq 0 ]  # The Command has no options
              then
                 display_man_section $COMMAND "SYNOPSIS" true true
            fi
         else
            case $ARG in
               --usageflag)
               FLAG_NAME="$2"
               display_flagname_information $COMMAND $FLAG_NAME
               shift
               shift
               COUNTER=`expr $COUNTER + 2`
               ;;
               --describe)
               display_man_section $COMMAND "DESCRIPTION" true
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
      echo "What manual page do you want?"
      usage
fi





