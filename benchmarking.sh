#!/bin/bash
rm output.txt
#test if the project builds successfully -- 40%
cd kernel_module
make 
cd ..

sudo insmod kernel_module/resource_container.ko
sudo chmod 777 /dev/rcontainer
#echo $@
#test if the projects works for serial/single process case. -- 10%
echo "Single Container -- serial" >> output.txt
./benchmark/benchmark 1 1 32768
cat *.log >> output.txt
rm -f *.log 

#test if the projects works for two containers with single process in each -- test data sharing module in isolation.--10%
echo "Two Containers -- isolation" >> output.txt
./benchmark/benchmark 2 1 32768 1 32768 
cat *.log >> output.txt
rm -f *.log 

#test if the projects works for one container with two processes in each -- test data sharing module in sharing. -- 10%
echo "One Container -- sharing" >> output.txt
./benchmark/benchmark 1 2 32768
cat *.log >> output.txt
rm -f *.log 

#test if the projects works for the given case in the project page -- 10%
echo "Default case" >> output.txt
./benchmark/benchmark 2 2 8192 1 16384 
cat *.log >> output.txt
rm -f *.log 

#test if the container works for larger cases -- 20%
echo "Scale up #1" >> output.txt
./benchmark/benchmark 2 2 65536 2 65536 
cat *.log >> output.txt
rm -f *.log 

echo "Scale up #2" >> output.txt
./benchmark/benchmark 2 4 65536 4 65536
cat *.log >> output.txt
rm -f *.log 

sudo rmmod resource_container