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
//     User APIs of CSE202's Resource Container Project in User Space
//
////////////////////////////////////////////////////////////////////////

#include "rcontainer.h"

int rcontainer_delete(int devfd)
{
    struct resource_container_cmd cmd;
    return ioctl(devfd, RCONTAINER_IOCTL_DELETE, &cmd);
}

/**
 * create function in user space that sends command to kernel space
 * for creating the current task in specified container.
 */
int rcontainer_create(int devfd, int cid)
{
    struct resource_container_cmd cmd;
    cmd.cid = cid;
    return ioctl(devfd, RCONTAINER_IOCTL_CREATE, &cmd);
}

/**
 * Allocate memory in kernel space for sharing along with tasks in the same container.
 */
void *rcontainer_heap_alloc(int devfd, __u64 offset, __u64 size)
{
    __u64 aligned_size = ((size + getpagesize() - 1) / getpagesize()) * getpagesize();
    return mmap(0, aligned_size, PROT_READ | PROT_WRITE, MAP_SHARED, devfd, offset * getpagesize());
}

/**
 * Lock a data item
 */
int rcontainer_lock(int devfd, __u64 offset)
{
    struct resource_container_cmd cmd;
    cmd.oid = offset;
    return ioctl(devfd, RCONTAINER_IOCTL_LOCK, &cmd);
}

/**
 * Unlock a data item
 */
int rcontainer_unlock(int devfd, __u64 offset)
{
    struct resource_container_cmd cmd;
    cmd.oid = offset;
    return ioctl(devfd, RCONTAINER_IOCTL_UNLOCK, &cmd);
}

/**
 * Invalidate the content of object.
 */
int rcontainer_free(int devfd, __u64 offset)
{
    struct resource_container_cmd cmd;
    cmd.oid = offset;
    return ioctl(devfd, RCONTAINER_IOCTL_FREE, &cmd);
}

int rcontainer_context_switch_handler(int devfd, int id)
{
     struct resource_container_cmd cmd;
     cmd.cid = id;
     return ioctl(DEVFD, RCONTAINER_IOCTL_CSWITCH, &cmd);
}
