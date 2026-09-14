#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <stdint.h>
#include "integrity.h"
#include "policy.h"

typedef enum {
    AUTH_UNINITIALIZED,
    AUTH_CONTEXT_RESOLUTION,
    AUTH_INTEGRITY_VALIDATION,
    AUTH_POLICY_VALIDATION,
    AUTH_OVERRIDE_PENDING,
    AUTH_COMMITTED,
    AUTH_REJECTED
} AUTH_STATE;

typedef enum {
    AUTH_REASON_NONE,
    AUTH_REASON_CONTEXT_INVALID,
    AUTH_REASON_INTEGRITY_FAILURE,
    AUTH_REASON_POLICY_FAILURE,
    AUTH_REASON_HOST_PROTECTED,
    AUTH_REASON_COMMIT_FAILURE
} AUTH_REASON;

typedef struct {
    AUTH_STATE state;
    AUTH_REASON reason;

    INTEGRITY_CONTEXT integrity;
    SECURITY_POLICY policy;

    uint64_t session_id;
    uint64_t authorization_key;
} AUTHORIZATION_SESSION;

void authorization_initialize(
    AUTHORIZATION_SESSION *session
);

int authorization_resolve_context(
    AUTHORIZATION_SESSION *session
);

int authorization_validate_integrity(
    AUTHORIZATION_SESSION *session
);

int authorization_validate_policy(
    AUTHORIZATION_SESSION *session
);

int authorization_request_override(
    AUTHORIZATION_SESSION *session
);

int authorization_commit(
    AUTHORIZATION_SESSION *session
);

int authorization_execute(
    AUTHORIZATION_SESSION *session
);

const char *authorization_state_name(
    AUTH_STATE state
);

const char *authorization_reason_name(
    AUTH_REASON reason
);

#endif
