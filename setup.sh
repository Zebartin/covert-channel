#! /bin/bash
useradd -m userlow
useradd -m userhigh
usermod -h USER_PUBLIC userlow
usermod -h USER_LOGIN userhigh
chmod 777 /home/userlow/
chmod 777 /home/userhigh/
mkdir /home/userlow/shared/
mkdir /home/userlow/lowlvl/
chmod 777 /home/userlow/shared/
chmod 777 /home/userlow/lowlvl/
setlevel USER_LOGIN /home/userhigh/
setlevel USER_PUBLIC /home/userlow/lowlvl/
echo OMICRON > /home/userhigh/in_text
