#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <stdint.h>

typedef struct {
    uint64_t total_units;
    uint64_t processed_units;
    uint64_t released_units;
    uint64_t elapsed_ms;
    unsigned int throughput_kbps;
} ALLOCATION_CONTEXT;

void allocation_initialize(
    ALLOCATION_CONTEXT *context,
    uint64_t size_kb
);

uint64_t allocation_get_total_units(
    const ALLOCATION_CONTEXT *context
);

uint64_t allocation_get_processed_units(
    const ALLOCATION_CONTEXT *context
);

uint64_t allocation_get_released_units(
    const ALLOCATION_CONTEXT *context
);

unsigned int allocation_get_progress(
    const ALLOCATION_CONTEXT *context
);

unsigned int allocation_get_delay(
    const ALLOCATION_CONTEXT *context
);

void allocation_process(
    ALLOCATION_CONTEXT *context,
    uint64_t units
);

void allocation_release(
    ALLOCATION_CONTEXT *context,
    uint64_t units
);

#endif
