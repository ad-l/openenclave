// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef _OE_POSIX_EPOLLOPS_H
#define _OE_POSIX_EPOLLOPS_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>
#include <openenclave/corelibc/sys/epoll.h>
#include <openenclave/internal/defs.h>
#include <openenclave/internal/device/deviceops.h>

OE_EXTERNC_BEGIN

struct oe_epoll_event;
struct oe_pollfd;
typedef struct _oe_device oe_device_t;

typedef struct _oe_epoll_ops
{
    oe_device_ops_t base;

    oe_device_t* (*create)(oe_device_t* epoll_device, int size);

    oe_device_t* (*create1)(oe_device_t* epoll_device, int flags);

    int (*ctl_add)(int epoll_fd, int enclave_fd, struct oe_epoll_event* event);

    int (*ctl_del)(int epoll_fd, int enclave_fd);

    int (*ctl_mod)(int epoll_fd, int enclave_fd, struct oe_epoll_event* event);

    int (*wait)(
        int epoll_fd,
        struct oe_epoll_event* events,
        size_t maxevents,
        int64_t timeout);

    int (*poll)(
        int epoll_fd,
        struct oe_pollfd* events,
        size_t maxevents,
        int64_t timeout);

    int (*add_event_data)(
        int epoll_fd,
        int enclave_fd,
        uint32_t events,
        uint64_t data);

    uint64_t (*get_event_data)(oe_device_t* epoll_device, uint32_t list_idx);

} oe_epoll_ops_t;

/*
**==============================================================================
**
** oe_ev_data_t:
**
**==============================================================================
*/

typedef union _oe_ev_data {
    struct
    {
        /* The epoll file descriptor on the enclave side. */
        int epfd;

        /* The corresponding index on the host side. */
        uint32_t list_idx;
    };
    uint64_t data;
} oe_ev_data_t;

/*
**==============================================================================
**
** oe_device_notifications_t:
**
**==============================================================================
*/

OE_PACK_BEGIN
typedef struct _oe_device_notifications /* overlays 'struct epoll_event' */
{
    uint32_t events;
    oe_ev_data_t data;
} oe_device_notifications_t;
OE_PACK_END

OE_STATIC_ASSERT(
    sizeof(oe_device_notifications_t) == sizeof(struct oe_epoll_event));

OE_EXTERNC_END

#endif /* _OE_POSIX_EPOLLOPS_H */