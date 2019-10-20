#!/bin/bash
echo "########## PHP installation"

SERVICE="php"
PKG_OK=$(dpkg-query -W --showformat='${Status}\n' php7.3 | grep "install ok installed")
if [[ "" == "$PKG_OK" ]] ; then
   echo -e "$SERVICE is not installed. Proceeding installation...\n"
   ./tools/update.sh 
   
   sudo apt install -y ca-certificates apt-transport-https
   sudo wget -q https://packages.sury.org/php/apt.gpg -O- | sudo apt-key add -
   echo "deb https://packages.sury.org/php/ stretch main" | sudo tee /etc/apt/sources.list.d/php.list
   sudo apt install -y php7.3
   sudo apt install -y php7.3-cli php7.3-common php7.3-curl php7.3-mbstring php7.3-mysql php7.3-xml
else 
   echo -e "$SERVICE is already installed!\n"
fi
