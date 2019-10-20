#!/bin/bash
echo "########## Emacs installation"

SERVICE="Emacs"
./tools/update.sh

PKG_OK=$(dpkg-query -W --showformat='${Status}\n' emacs | grep "install ok installed")
if [[ "" == "$PKG_OK" ]] ; then
   echo -e "$SERVICE is not installed. Proceeding installation...\n"
   sudo apt install -y emacs 
else 
   echo -e "$SERVICE is already installed!\n"
fi


# Configure it with the ETNA configuration
