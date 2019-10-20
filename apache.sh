#!/bin/bash
echo "########## Apache installation"

SERVICE="apache2"
PKG_OK=$(dpkg-query -W --showformat='${Status}\n' apache2 | grep "install ok installed")
if [[ "" == "$PKG_OK" ]] ; then
   echo -e "$SERVICE is not installed. Proceeding installation...\n"
   
   # Install php if necessary 
   ./web/php.sh
   
   # Install apache 2
   sudo apt install -y apache2
   # Create index.jsp 
   echo "<?php phpinfo();" > /var/www/html/index.php
else 
   echo -e "$SERVICE is already installed!\n"
fi


