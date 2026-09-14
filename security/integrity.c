#include "integrity.h"
#include <windows.h>

#define CONTEXT_VERSION 0x00070021
#define CONTEXT_REVISION 0x0042A91C

#define CONTEXT_HOST_BOUND       0x01
#define CONTEXT_INTEGRITY_LOCKED 0x02
#define CONTEXT_PROTECTED       0x04

static uint64_t rotate_left(
    uint64_t value,
    unsigned int bits
) {
    return (value << bits) |
           (value >> (64 - bits));
}

static uint64_t derive_signature(
    uint64_t value
) {
    value ^= 0x9E3779B97F4A7C15ULL;

    value = rotate_left(value, 17);

    value *= 0xBF58476D1CE4E5B9ULL;

    value ^= value >> 29;

    value = rotate_left(value, 31);

    value *= 0x94D049BB133111EBULL;

    value ^= value >> 32;

    return value;
}

void integrity_initialize(
    INTEGRITY_CONTEXT *context
) {
    if (context == NULL)
        return;

    context->version = CONTEXT_VERSION;
    context->revision = CONTEXT_REVISION;

    context->flags =
        CONTEXT_HOST_BOUND |
        CONTEXT_INTEGRITY_LOCKED |
        CONTEXT_PROTECTED;

    context->process_id =
        (uint64_t)GetCurrentProcessId();

    context->host_signature =
        derive_signature(
            context->process_id
        );

    context->context_signature =
        derive_signature(
            context->host_signature
        );

    context->state = INTEGRITY_INITIALIZED;
}

int integrity_validate(
    INTEGRITY_CONTEXT *context
) {
    if (context == NULL)
        return 0;

    if (context->state != INTEGRITY_INITIALIZED)
        return 0;

    uint64_t expected =
        derive_signature(
            context->host_signature
        );

    if (expected != context->context_signature) {
        context->state = INTEGRITY_REJECTED;
        return 0;
    }

    if ((context->flags &
         CONTEXT_INTEGRITY_LOCKED) == 0) {

        context->state = INTEGRITY_REJECTED;
        return 0;
    }

    context->state = INTEGRITY_VALID;

    if (context->flags &
        CONTEXT_PROTECTED) {

        context->state = INTEGRITY_PROTECTED;
    }

    return 1;
}

int integrity_is_protected(
    const INTEGRITY_CONTEXT *context
) {
    if (context == NULL)
        return 0;

    return context->state == INTEGRITY_PROTECTED;
}

const char *integrity_state_name(
    INTEGRITY_STATE state
) {
    switch (state) {
        case INTEGRITY_UNKNOWN:
            return "UNKNOWN";

        case INTEGRITY_INITIALIZED:
            return "INITIALIZED";

        case INTEGRITY_VALID:
            return "VALID";

        case INTEGRITY_PROTECTED:
            return "PROTECTED";

        case INTEGRITY_REJECTED:
            return "REJECTED";

        default:
            return "INVALID";
    }
}

uint64_t integrity_get_signature(
    const INTEGRITY_CONTEXT *context
) {
    if (context == NULL)
        return 0;

    return context->context_signature;
}