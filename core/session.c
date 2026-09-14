#include "session.h"

#include <windows.h>

static uint64_t generate_session_id(void) {
    uint64_t value;
    LARGE_INTEGER counter;

    value =
        (uint64_t)GetCurrentProcessId();

    if (QueryPerformanceCounter(&counter)) {
        value ^= (uint64_t)counter.QuadPart;
    }

    value ^= (uint64_t)time(NULL);

    value ^= 0xC6BC279692B5CC83ULL;

    value *= 0x9E3779B185EBCA87ULL;

    value ^= value >> 32;
    value *= 0xBF58476D1CE4E5B9ULL;
    value ^= value >> 29;

    return value;
}

void session_initialize(
    OPERATION_SESSION *session
) {
    if (session == NULL)
        return;

    ZeroMemory(
        session,
        sizeof(OPERATION_SESSION)
    );

    session->id =
        generate_session_id();

    session->state =
        SESSION_CREATED;

    session->start_time = 0;
    session->end_time = 0;

    session->current_component = 0;

    session->completed_components = 0;
    session->blocked_components = 0;

    session->processed_kb = 0;
    session->released_kb = 0;

    session->authorization_result = 0;
    session->operation_result = 0;

    inventory_initialize(
        &session->inventory
    );

    filesystem_initialize(
        &session->filesystem,
        0
    );

    allocation_initialize(
        &session->allocation,
        0
    );

    authorization_initialize(
        &session->authorization
    );
}

void session_start(
    OPERATION_SESSION *session
) {
    if (session == NULL)
        return;

    if (session->state != SESSION_CREATED)
        return;

    session->start_time = time(NULL);

    session->state =
        SESSION_PREPARING;
}

void session_prepare(
    OPERATION_SESSION *session
) {
    if (session == NULL)
        return;

    if (session->state != SESSION_PREPARING)
        return;

    inventory_scan(
        &session->inventory
    );

    filesystem_initialize(
        &session->filesystem,
        inventory_total_size(
            &session->inventory
        ) * 1024
    );

    allocation_initialize(
        &session->allocation,
        inventory_total_size(
            &session->inventory
        ) * 1024
    );
}

void session_begin(
    OPERATION_SESSION *session
) {
    if (session == NULL)
        return;

    if (session->state != SESSION_PREPARING)
        return;

    session->state =
        SESSION_RUNNING;

    filesystem_mount(
        &session->filesystem
    );

    filesystem_scan(
        &session->filesystem
    );

    filesystem_lock(
        &session->filesystem
    );
}

void session_finalize(
    OPERATION_SESSION *session
) {
    if (session == NULL)
        return;

    if (session->state != SESSION_RUNNING)
        return;

    session->state =
        SESSION_FINALIZING;

    session->end_time = time(NULL);
}

void session_abort(
    OPERATION_SESSION *session
) {
    if (session == NULL)
        return;

    if (session->state ==
        SESSION_COMPLETED)
        return;

    session->state =
        SESSION_ABORTED;

    session->operation_result = 0;

    session->end_time = time(NULL);

    filesystem_abort(
        &session->filesystem
    );
}

void session_complete(
    OPERATION_SESSION *session
) {
    if (session == NULL)
        return;

    if (session->state != SESSION_FINALIZING)
        return;

    session->state =
        SESSION_COMPLETED;

    session->operation_result = 1;

    session->end_time = time(NULL);
}

int session_is_active(
    const OPERATION_SESSION *session
) {
    if (session == NULL)
        return 0;

    return
        session->state == SESSION_CREATED ||
        session->state == SESSION_PREPARING ||
        session->state == SESSION_RUNNING ||
        session->state == SESSION_FINALIZING;
}

int session_is_finished(
    const OPERATION_SESSION *session
) {
    if (session == NULL)
        return 0;

    return
        session->state == SESSION_ABORTED ||
        session->state == SESSION_COMPLETED;
}

const char *session_state_name(
    SESSION_STATE state
) {
    switch (state) {
        case SESSION_UNINITIALIZED:
            return "UNINITIALIZED";

        case SESSION_CREATED:
            return "CREATED";

        case SESSION_PREPARING:
            return "PREPARING";

        case SESSION_RUNNING:
            return "RUNNING";

        case SESSION_FINALIZING:
            return "FINALIZING";

        case SESSION_ABORTED:
            return "ABORTED";

        case SESSION_COMPLETED:
            return "COMPLETED";

        default:
            return "UNKNOWN";
    }
}

uint64_t session_get_id(
    const OPERATION_SESSION *session
) {
    if (session == NULL)
        return 0;

    return session->id;
}