#ifndef SESSION_H
#define SESSION_H

#include <stdint.h>
#include <time.h>

#include "../security/authorization.h"
#include "../system/inventory.h"
#include "../storage/filesystem.h"
#include "../storage/allocation.h"

typedef enum {
    SESSION_UNINITIALIZED,
    SESSION_CREATED,
    SESSION_PREPARING,
    SESSION_RUNNING,
    SESSION_FINALIZING,
    SESSION_ABORTED,
    SESSION_COMPLETED
} SESSION_STATE;

typedef struct {
    uint64_t id;

    time_t start_time;
    time_t end_time;

    SESSION_STATE state;

    SYSTEM_INVENTORY inventory;
    FILESYSTEM_CONTEXT filesystem;
    ALLOCATION_CONTEXT allocation;

    AUTHORIZATION_SESSION authorization;

    size_t current_component;
    unsigned int completed_components;
    unsigned int blocked_components;

    uint64_t processed_kb;
    uint64_t released_kb;

    int authorization_result;
    int operation_result;
} OPERATION_SESSION;

void session_initialize(
    OPERATION_SESSION *session
);

void session_start(
    OPERATION_SESSION *session
);

void session_prepare(
    OPERATION_SESSION *session
);

void session_begin(
    OPERATION_SESSION *session
);

void session_finalize(
    OPERATION_SESSION *session
);

void session_abort(
    OPERATION_SESSION *session
);

void session_complete(
    OPERATION_SESSION *session
);

int session_is_active(
    const OPERATION_SESSION *session
);

int session_is_finished(
    const OPERATION_SESSION *session
);

const char *session_state_name(
    SESSION_STATE state
);

uint64_t session_get_id(
    const OPERATION_SESSION *session
);

#endif
