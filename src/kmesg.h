#ifndef _DUO_KMESG_H
#define _DUO_KMESG_H

#include <sys/types.h>

ssize_t kmesg_write(const void *buf, size_t nbyte);

#endif // ! _DUO_KMESG_H
