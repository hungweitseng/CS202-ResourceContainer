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


#ifndef RESOURCE_CONTAINER_H
#define RESOURCE_CONTAINER_H

#include <linux/types.h>


struct resource_container_cmd {
    __u64 op;
    __u64 cid;
    __u64 oid;
};

struct mapping_entry
{
    void *page;
    struct mapping_entry *next_page;
    __u64 oid;
    int size;
};
// 32 bytes
struct task_struct_node 
{
    struct task_struct *task;
    struct task_struct_node *next_task;
    __u64 valid;
    __u64 dummy; // just for the page alignment.
};


#define RCONTAINER_IOCTL_LOCK  _IOWR('N', 0x43, struct resource_container_cmd)
#define RCONTAINER_IOCTL_UNLOCK  _IOWR('N', 0x44, struct resource_container_cmd)
#define RCONTAINER_IOCTL_DELETE  _IOWR('N', 0x45, struct resource_container_cmd)
#define RCONTAINER_IOCTL_CREATE  _IOWR('N', 0x46, struct resource_container_cmd)
#define RCONTAINER_IOCTL_CSWITCH  _IOWR('N', 0x47, struct resource_container_cmd)
#define RCONTAINER_IOCTL_FREE _IOWR('N', 0x48, struct resource_container_cmd)

#endif
