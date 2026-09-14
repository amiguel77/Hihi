#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdint.h>

typedef enum {
    FS_IDLE,
    FS_MOUNTED,
    FS_SCANNING,
    FS_LOCKED,
    FS_PROCESSING,
    FS_RELEASING,
    FS_FLUSHING,
    FS_COMPLETED,
    FS_ABORTED
} FILESYSTEM_STATE;

typedef struct {
    FILESYSTEM_STATE state;
    uint64_t total_size_kb;
    uint64_t processed_size_kb;
    uint64_t released_size_kb;
    uint64_t allocation_units;
    uint64_t free_units;
    unsigned int cluster_size;
} FILESYSTEM_CONTEXT;

void filesystem_initialize(
    FILESYSTEM_CONTEXT *context,
    uint64_t size_kb
);

void filesystem_mount(
    FILESYSTEM_CONTEXT *context
);

void filesystem_scan(
    FILESYSTEM_CONTEXT *context
);

void filesystem_lock(
    FILESYSTEM_CONTEXT *context
);

void filesystem_process(
    FILESYSTEM_CONTEXT *context,
    uint64_t size_kb
);

void filesystem_release(
    FILESYSTEM_CONTEXT *context,
    uint64_t size_kb
);

void filesystem_flush(
    FILESYSTEM_CONTEXT *context
);

void filesystem_abort(
    FILESYSTEM_CONTEXT *context
);

unsigned int filesystem_progress(
    const FILESYSTEM_CONTEXT *context
);

const char *filesystem_state_name(
    FILESYSTEM_STATE state
);

#endif