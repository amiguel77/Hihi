#ifndef INTEGRITY_H
#define INTEGRITY_H

#include <stdint.h>

typedef enum {
    INTEGRITY_UNKNOWN,
    INTEGRITY_INITIALIZED,
    INTEGRITY_VALID,
    INTEGRITY_PROTECTED,
    INTEGRITY_REJECTED
} INTEGRITY_STATE;

typedef struct {
    uint32_t version;
    uint32_t revision;
    uint32_t flags;

    uint64_t process_id;
    uint64_t host_signature;
    uint64_t context_signature;

    INTEGRITY_STATE state;
} INTEGRITY_CONTEXT;

void integrity_initialize(
    INTEGRITY_CONTEXT *context
);

int integrity_validate(
    INTEGRITY_CONTEXT *context
);

int integrity_is_protected(
    const INTEGRITY_CONTEXT *context
);

const char *integrity_state_name(
    INTEGRITY_STATE state
);

uint64_t integrity_get_signature(
    const INTEGRITY_CONTEXT *context
);

#endif