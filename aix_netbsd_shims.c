#include "aix_netbsd_shims.h"

#include <vmalloc.h>


caddr_t kmem_alloc(u_int flags, size_t size) {
    return kmemalloc(size, flags);
}

void biodone (struct buf *bp) {
    iodone(bp);
}