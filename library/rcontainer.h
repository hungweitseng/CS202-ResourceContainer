//////////////////////////////////////////////////////////////////////
//                      University of California, Riverside
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
//     Header File of CSE202's Resource Container Project in User Space
//
////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

#include <resource_container/resource_container.h>
#include <linux/types.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int rcontainer_delete(int devfd);
int rcontainer_create(int devfd, int cid);
//int rcontainer_context_switch_handler(int devfd, int cid);
int rcontainer_context_switch_handler(int devfd, int cid);
int rcontainer_init(int devfd);
void *rcontainer_heap_alloc(int devfd, __u64 offset, __u64 size);
int rcontainer_lock(int devfd, __u64 offset);
int rcontainer_unlock(int devfd, __u64 offset);
int rcontainer_free(int devfd, __u64 offset);
    
int DEVFD;
static void handler(int sig, siginfo_t *si, void *unused) {
    int devfd;
    int cid;
//    fprintf(stderr,"sigalarm\n");
    rcontainer_context_switch_handler(0, 0);
}

int rcontainer_init(int devfd)
{
    struct sigaction sa;
    struct itimerval timeout;
    
    sa.sa_flags = SA_SIGINFO|SA_RESTART|SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handler;
    if (sigaction(SIGPROF, &sa, NULL) == -1) {
        fprintf(stderr,"sigaction");
        exit(1);
    }
    
    DEVFD=devfd;
    timeout.it_value.tv_sec = 0;
    timeout.it_value.tv_usec = 5;
    timeout.it_interval = timeout.it_value;
    if(setitimer(ITIMER_PROF, &timeout, NULL)==1)
        fprintf(stderr,"Timer failed\n");
    return 0;
}

#ifdef __cplusplus
}
#endif