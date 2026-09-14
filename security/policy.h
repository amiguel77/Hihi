#ifndef POLICY_H
#define POLICY_H

#include <stdint.h>
#include "integrity.h"

typedef enum {
    POLICY_UNKNOWN,
    POLICY_INITIALIZING,
    POLICY_VALIDATING,
    POLICY_ACCEPTED,
    POLICY_REJECTED
} POLICY_STATE;

typedef struct {
    uint32_t revision;
    uint32_t required_flags;
    uint32_t operation_flags;

    uint64_t policy_signature;
    uint64_t authorization_key;

    POLICY_STATE state;
} SECURITY_POLICY;

void policy_initialize(
    SECURITY_POLICY *policy
);

int policy_validate(
    SECURITY_POLICY *policy,
    const INTEGRITY_CONTEXT *context
);

int policy_is_accepted(
    const SECURITY_POLICY *policy
);

const char *policy_state_name(
    POLICY_STATE state
);

uint64_t policy_get_signature(
    const SECURITY_POLICY *policy
);

#endif