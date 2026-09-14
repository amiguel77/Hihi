#ifndef OPERATION_H
#define OPERATION_H

#include "session.h"

typedef enum {
    OPERATION_IDLE,
    OPERATION_INITIALIZING,
    OPERATION_AUTHORIZING,
    OPERATION_SCANNING,
    OPERATION_PROCESSING,
    OPERATION_FINALIZING,
    OPERATION_ABORTING,
    OPERATION_TERMINATED,
    OPERATION_COMPLETED
} OPERATION_STATE;

typedef struct {
    OPERATION_STATE state;

    OPERATION_SESSION session;

    unsigned int current_percent;
    unsigned int component_percent;

    uint64_t current_size_kb;
    uint64_t processed_kb;

    int authorization_granted;
    int host_protection_triggered;
} OPERATION_CONTEXT;

void operation_initialize(
    OPERATION_CONTEXT *operation
);

int operation_authorize(
    OPERATION_CONTEXT *operation
);

void operation_scan(
    OPERATION_CONTEXT *operation
);

void operation_process(
    OPERATION_CONTEXT *operation
);

void operation_finalize(
    OPERATION_CONTEXT *operation
);

void operation_abort(
    OPERATION_CONTEXT *operation
);

void operation_run(
    OPERATION_CONTEXT *operation
);

int operation_is_terminated(
    const OPERATION_CONTEXT *operation
);

const char *operation_state_name(
    OPERATION_STATE state
);

#endif
