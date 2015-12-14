/** @file   ublkdev.h
 *  @brief  General (userland) header for userspace block devices.
 */

#ifndef UBLKDEV_H
#define UBLKDEV_H

#include <linux/genhd.h>
#include <linux/types.h>

/** ubd ioctl magic number -- seems unused */
#define UBD_IOC_MAGIC           0xbf

/** ioctl: register block device. */
#define UBD_IOCREGISTER     _IOWR(UBD_IOC_MAGIC, 0xa0, struct ubd_info)

/** ioctl: unregister block device. */
#define UBD_IOCUNREGISTER   _IOW(UBD_IOC_MAGIC, 0xa1, int)

/** ioctl: get registered block device count. */
#define UBD_IOCGETCOUNT     _IOR(UBD_IOC_MAGIC, 0xa2, int)

/** ioctl: describe block device. */
#define UBD_IOCDESCRIBE     _IOWR(UBD_IOC_MAGIC, 0xa3, struct ubd_describe)

/** ioctl: tie control to block device. */
#define UBD_IOCTIE          _IOW(UBD_IOC_MAGIC, 0xa4, struct ubd_tie)

/** ioctl: get next request. */
#define UBD_IOCGETREQUEST   _IOWR(UBD_IOC_MAGIC, 0xa5, struct ubd_message)

/** ioctl: put reply. */
#define UBD_IOCPUTREPLY     _IOW(UBD_IOC_MAGIC, 0xa6, struct ubd_message)

/** ubd_flags: device is read only. */
#define UBD_FL_READ_ONLY    0x00000001

/** ubd_flags: device is a removable disk. */
#define UBD_FL_REMOVABLE    0x00000002

/** Information about a userspace block device. */
struct ubd_info {
    /** Device name */
    char ubd_name[DISK_NAME_LEN];

    /** Device flags */
    uint32_t ubd_flags;

    /** Major number */
    uint32_t ubd_major;

    /** Device capacity in 512-byte sectors. */
    uint64_t ubd_nsectors;
};

struct ubd_describe {
    /** [IN] The index to return. */
    size_t ubd_index;

    /** [OUT] The resulting ubd_info data. */
    struct ubd_info ubd_info;
};

struct ubd_tie {
    /** [IN] The major mode of the device to tie to. */
    uint32_t mbd_major;

    /** [IN] The name of the device to tie to (if ubd_major is zero). */
    char ubd_name[DISK_NAME_LEN];
};

/** ubd_msgtype code for replies. */
#define UBD_MSGTYPE_REPLY                   0x80000000

/** ubd_msgtype code for a read request/reply. */
#define UBD_MSGTYPE_READ                    0

/** ubd_msgtype code for a write request/reply. */
#define UBD_MSGTYPE_WRITE                   1

/** ubd_msgtype code for a discard (trim) request. */
#define UBD_MSGTYPE_DISCARD                 2

/** Maximum size of a UBD message */
#define UBD_MAX_MESSAGE_SIZE                ((size_t) (4 << 20))   /* 4 MB */

/** UBD request / reply
 *  
 *  Requests are made by the driver and sent to userspace when the handler
 *  invokes the @c UBD_IOCGETREQUEST @c ioctl.  Replies are sent from userspace
 *  through a @c UBD_IOCPUTREQUEST @c ioctl.
 */
struct ubd_message {
    /** The type of this message.
     *
     *  @li @c UBD_IOCGETREQUEST [OUT]
     *  @li @c UBD_IOCPUTREPLY [IN]
     */
    uint32_t ubd_msgtype;
    
    /** Tag to match up requests with replies.
     *
     *  @li @c UBD_IOCGETREQUEST [OUT]
     *  @li @c UBD_IOCPUTREPLY [IN]
     */
    uint32_t ubd_tag;

    /** Number of 512-byte sectors to read.
     *
     *  @li @c UBD_IOCGETREQUEST [OUT]
     *  @li @c UBD_IOCPUTREPLY Unused
     */
    uint32_t ubd_nsectors;
    
    /** First sector of the request.
     *
     *  @li @c UBD_IOCGETREQUEST [OUT]
     *  @li @c UBD_IOCPUTREPLY Unused
     */
    uint64_t ubd_first_sector;

    /** Number of bytes in @c ubd_data.
     *
     *  @li @c UBD_IOCGETREQUEST [IN OUT] When called, this indicates the
     *      capacity of @c ubd_data.  Upon return, this indicates the number
     *      of valid bytes in @c ubd_data (zero for read and trim requests;
     *      non-zero for write requests).
     *  @li @c UBD_IOCPUTREPLY [IN] The number of valid bytes in @c ubd_data.
     *      This is valid only for write replies.
     */

    /** Data for this request. */
    char ubd_data[0];
};

#endif /* UBLKDEV_H */

/* Local variables: */
/* mode: C */
/* indent-tabs-mode: nil */
/* tab-width: 8 */
/* End: */
/* vi: set expandtab tabstop=8 */
