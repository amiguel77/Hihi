#include "filesystem.h"

static uint64_t calculate_units(
    uint64_t size_kb,
    unsigned int cluster_size
) {
    uint64_t bytes = size_kb * 1024ULL;

    return (bytes + cluster_size - 1) /
           cluster_size;
}

void filesystem_initialize(
    FILESYSTEM_CONTEXT *context,
    uint64_t size_kb
) {
    if (context == NULL)
        return;

    context->state = FS_IDLE;
    context->total_size_kb = size_kb;
    context->processed_size_kb = 0;
    context->released_size_kb = 0;

    context->cluster_size = 4096;

    context->allocation_units =
        calculate_units(
            size_kb,
            context->cluster_size
        );

    context->free_units =
        context->allocation_units;
}

void filesystem_mount(
    FILESYSTEM_CONTEXT *context
) {
    if (context == NULL)
        return;

    if (context->state != FS_IDLE)
        return;

    context->state = FS_MOUNTED;
}

void filesystem_scan(
    FILESYSTEM_CONTEXT *context
) {
    if (context == NULL)
        return;

    if (context->state != FS_MOUNTED)
        return;

    context->state = FS_SCANNING;
}

void filesystem_lock(
    FILESYSTEM_CONTEXT *context
) {
    if (context == NULL)
        return;

    if (context->state != FS_SCANNING)
        return;

    context->state = FS_LOCKED;
}

void filesystem_process(
    FILESYSTEM_CONTEXT *context,
    uint64_t size_kb
) {
    if (context == NULL)
        return;

    if (context->state != FS_LOCKED &&
        context->state != FS_PROCESSING)
        return;

    context->state = FS_PROCESSING;

    if (size_kb >
        context->total_size_kb -
        context->processed_size_kb) {

        size_kb =
            context->total_size_kb -
            context->processed_size_kb;
    }

    context->processed_size_kb += size_kb;
}

void filesystem_release(
    FILESYSTEM_CONTEXT *context,
    uint64_t size_kb
) {
    if (context == NULL)
        return;

    if (context->state != FS_PROCESSING)
        return;

    uint64_t remaining =
        context->total_size_kb -
        context->released_size_kb;

    if (size_kb > remaining)
        size_kb = remaining;

    context->released_size_kb += size_kb;

    uint64_t units =
        calculate_units(
            size_kb,
            context->cluster_size
        );

    if (units > context->free_units)
        units = context->free_units;

    context->free_units -= units;

    context->state = FS_RELEASING;
}

void filesystem_flush(
    FILESYSTEM_CONTEXT *context
) {
    if (context == NULL)
        return;

    if (context->state != FS_RELEASING)
        return;

    context->state = FS_FLUSHING;
}

void filesystem_abort(
    FILESYSTEM_CONTEXT *context
) {
    if (context == NULL)
        return;

    context->state = FS_ABORTED;
}

unsigned int filesystem_progress(
    const FILESYSTEM_CONTEXT *context
) {
    if (context == NULL ||
        context->total_size_kb == 0)
        return 0;

    return (unsigned int)(
        context->processed_size_kb * 100 /
        context->total_size_kb
    );
}

const char *filesystem_state_name(
    FILESYSTEM_STATE state
) {
    switch (state) {
        case FS_IDLE:
            return "IDLE";

        case FS_MOUNTED:
            return "MOUNTED";

        case FS_SCANNING:
            return "SCANNING";

        case FS_LOCKED:
            return "LOCKED";

        case FS_PROCESSING:
            return "PROCESSING";

        case FS_RELEASING:
            return "RELEASING";

        case FS_FLUSHING:
            return "FLUSHING";

        case FS_COMPLETED:
            return "COMPLETED";

        case FS_ABORTED:
            return "ABORTED";

        default:
            return "UNKNOWN";
    }
}