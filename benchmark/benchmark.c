//////////////////////////////////////////////////////////////////////
//                     University of California, Riverside
//
//
//
//                             Copyright 2021
//
////////////////////////////////////////////////////////////////////////
//
// This program is free software; you can redistribute it and/or modify it
// under the terms and conditions of the GNU General Public License,
// version 2, as published by the Free Software Foundation.
//
// This program is distributed in the hope it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
//
////////////////////////////////////////////////////////////////////////
//
//   Author:  Hung-Wei Tseng, Yu-Chia Liu
//
//   Description:
//     Running Applications on Resource Containers
//
////////////////////////////////////////////////////////////////////////

#include <rcontainer.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/syscall.h>

struct container_info
{
    int number_of_processes;
    int heap_size;
};

int main(int argc, char *argv[])
{
    // variable initialization
    int i = 0; 
    int total_number_of_processes = 0, heap_size = 8192, number_of_containers = 1;
    int a, j, k, cid, size, stat, child_pid, devfd, max_size_of_objects_with_buffer;
    char filename[256];
    int *mapped_data, *data, *current_value, progress=0, sum=0;
    unsigned long long msec_time;
    FILE *fp;
    struct timeval current_time;
    struct container_info *containers;
    pid_t *pid; 
    int max;
    // takes arguments from command line interface.
    if (argc < 4)
    {
        fprintf(stderr, "Usage: %s number_of_containers number_of_threads_in_container_#1 share_heap_size_in_container_#1 ...\n", argv[0]);
        exit(1);
    }

    number_of_containers = atoi(argv[1]);
    containers = calloc(number_of_containers, sizeof(struct container_info));
    for(i = 0; i < number_of_containers; i++)
    {
        containers[i].number_of_processes = atoi(argv[i*2+2]);
        containers[i].heap_size = atoi(argv[i*2+3]);
        total_number_of_processes += containers[i].number_of_processes;
    }

    pid = (pid_t *) calloc(total_number_of_processes, sizeof(pid_t));

    // open the kernel module to use it
    devfd = open("/dev/rcontainer", O_RDWR);
    if (devfd < 0)
    {
        fprintf(stderr, "Device open failed");
        exit(1);
    }
    k =0;
    // parent process forks children
    for (i = 0; i < (number_of_containers); i++)
    {
        for(j=0;j<containers[i].number_of_processes;j++)
        {
            child_pid = fork();
            if (child_pid == 0)
            {
                break;
            }
            else
            {
                pid[k++] = child_pid;
            }
        }
            if (child_pid == 0)
            {
                break;
            }
    }
    cid = i;

    // create the log file
    if(child_pid == 0)
    {
    srand((int)time(NULL) + (int)getpid());
    sprintf(filename, "rcontainer.%d.log", (int)getpid());
    fp = fopen(filename, "w");

    // create/link this process to a container.
    rcontainer_create(devfd, cid);
    rcontainer_lock(devfd, 0);
    current_value = (int *)rcontainer_heap_alloc(devfd, 0, sizeof(int));
    *current_value = 0;
    rcontainer_unlock(devfd, 0);
    
    max = containers[i].heap_size/sizeof(int);
    
    // Writing to objects
    for (i = 0 ; *current_value < max; i++)
    {
        rcontainer_lock(devfd, 1);
        mapped_data = (int *)rcontainer_heap_alloc(devfd, 1, heap_size);

        // error handling
        if (!mapped_data)
        {
            fprintf(stderr, "Failed in container_heap_alloc()\n");
            exit(1);
        }

        // generate a random number to write into the object.
        mapped_data[*current_value]=*current_value;
        *current_value = *current_value + 1;
        progress++;
        // prints out the result into the log
        rcontainer_unlock(devfd, 1);
    }
    for (i = 0; i < max; i++)
    {
        sum += mapped_data[i];
    }
    fprintf(fp, "Process: %d in container %d produces %d numbers in the heap. Heap checksum is %d. Should be %d\n", getpid(), cid, progress, sum, (max-1)*(max)/2);
    // try delete something
    rcontainer_lock(devfd, 1);
    rcontainer_free(devfd, 1);
    rcontainer_unlock(devfd, 1);
    rcontainer_lock(devfd, 0);
    rcontainer_free(devfd,0);
    rcontainer_unlock(devfd, 0);
    
    // done with works, cleanup and wait for other processes.
    rcontainer_delete(devfd);
    close(devfd);
    }
    else
    {
        for (i = 0; i < (total_number_of_processes); i++)
        {
            waitpid(pid[i], &stat, 0);  
        }
    }
    free(pid);
    free(data);
    return 0;
}
