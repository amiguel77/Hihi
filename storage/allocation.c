#include "allocation.h"
#include <stdlib.h>
#include <time.h>

static uint64_t clamp_units(
    uint64_t value,
    uint64_t maximum
) {
    if (value > maximum)
        return maximum;

    return value;
}

void allocation_initialize(
    ALLOCATION_CONTEXT *context,
    uint64_t size_kb
) {
    if (context == NULL)
        return;

    context->total_units =
        size_kb * 4;

    if (context->total_units == 0)
        context->total_units = 1;

    context->processed_units = 0;
    context->released_units = 0;
    context->elapsed_ms = 0;

    if (size_kb < 1024) {
        context->throughput_kbps = 850;
    } else if (size_kb < 4096) {
        context->throughput_kbps = 1200;
    } else if (size_kb < 8192) {
        context->throughput_kbps = 1800;
    } else if (size_kb < 16384) {
        context->throughput_kbps = 2400;
    } else {
        context->throughput_kbps = 3200;
    }
}

uint64_t allocation_get_total_units(
    const ALLOCATION_CONTEXT *context
) {
    if (context == NULL)
        return 0;

    return context->total_units;
}

uint64_t allocation_get_processed_units(
    const ALLOCATION_CONTEXT *context
) {
    if (context == NULL)
        return 0;

    return context->processed_units;
}

uint64_t allocation_get_released_units(
    const ALLOCATION_CONTEXT *context
) {
    if (context == NULL)
        return 0;

    return context->released_units;
}

unsigned int allocation_get_progress(
    const ALLOCATION_CONTEXT *context
) {
    if (context == NULL ||
        context->total_units == 0)
        return 0;

    return (unsigned int)(
        context->processed_units * 100 /
        context->total_units
    );
}

unsigned int allocation_get_delay(
    const ALLOCATION_CONTEXT *context
) {
    if (context == NULL)
        return 100;

    unsigned int base =
        100000 / context->throughput_kbps;

    unsigned int variation =
        rand() % (base + 1);

    return base + variation + 20;
}

void allocation_process(
    ALLOCATION_CONTEXT *context,
    uint64_t units
) {
    if (context == NULL)
        return;

    context->processed_units =
        clamp_units(
            context->processed_units + units,
            context->total_units
        );

    context->elapsed_ms +=
        allocation_get_delay(context);
}

void allocation_release(
    ALLOCATION_CONTEXT *context,
    uint64_t units
) {
    if (context == NULL)
        return;

    context->released_units =
        clamp_units(
            context->released_units + units,
            context->total_units
        );
}