#!/bin/bash

# Parse input
#if [ $# -ne 4 ]; then
#    echo "Usage: $0 <# of objects> <max size of objects> <# of tasks> <# of containers>"
#    exit
#fi

sudo insmod kernel_module/resource_container.ko
sudo chmod 777 /dev/rcontainer
#echo $@
./benchmark/benchmark "$@"
cat *.log 

# if you want to see the log for debugging, comment out the following line.
rm -f *.log 

sudo rmmod resource_container