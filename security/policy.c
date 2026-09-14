#include "policy.h"

#define POLICY_REVISION 0x0042A91C

#define POLICY_HOST_BOUND       0x01
#define POLICY_INTEGRITY_LOCKED 0x02
#define POLICY_PROTECTED        0x04

#define OPERATION_SYSTEM_SCOPE  0x01
#define OPERATION_RECURSIVE     0x02
#define OPERATION_EXCLUSIVE     0x04

static uint64_t rotate_left(
    uint64_t value,
    unsigned int bits
) {
    return (value << bits) |
           (value >> (64 - bits));
}

static uint64_t derive_policy_signature(
    uint64_t seed
) {
    seed ^= 0xA0761D6478BD642FULL;

    seed = rotate_left(seed, 19);

    seed *= 0xE7037ED1A0B428DBULL;

    seed ^= seed >> 27;

    seed = rotate_left(seed, 37);

    seed *= 0x8EBC6AF09C88C6E3ULL;

    seed ^= seed >> 31;

    return seed;
}

void policy_initialize(
    SECURITY_POLICY *policy
) {
    if (policy == NULL)
        return;

    policy->revision = POLICY_REVISION;

    policy->required_flags =
        POLICY_HOST_BOUND |
        POLICY_INTEGRITY_LOCKED |
        POLICY_PROTECTED;

    policy->operation_flags =
        OPERATION_SYSTEM_SCOPE |
        OPERATION_RECURSIVE |
        OPERATION_EXCLUSIVE;

    policy->policy_signature =
        derive_policy_signature(
            POLICY_REVISION
        );

    policy->authorization_key =
        derive_policy_signature(
            policy->policy_signature
        );

    policy->state = POLICY_INITIALIZING;
}

int policy_validate(
    SECURITY_POLICY *policy,
    const INTEGRITY_CONTEXT *context
) {
    if (policy == NULL || context == NULL)
        return 0;

    if (policy->state != POLICY_INITIALIZING)
        return 0;

    policy->state = POLICY_VALIDATING;

    if (context->state != INTEGRITY_PROTECTED) {
        policy->state = POLICY_REJECTED;
        return 0;
    }

    if ((context->flags &
         policy->required_flags) !=
        policy->required_flags) {

        policy->state = POLICY_REJECTED;
        return 0;
    }

    uint64_t expected =
        derive_policy_signature(
            POLICY_REVISION
        );

    if (expected != policy->policy_signature) {
        policy->state = POLICY_REJECTED;
        return 0;
    }

    uint64_t expected_key =
        derive_policy_signature(
            policy->policy_signature
        );

    if (expected_key != policy->authorization_key) {
        policy->state = POLICY_REJECTED;
        return 0;
    }

    policy->state = POLICY_ACCEPTED;

    return 1;
}

int policy_is_accepted(
    const SECURITY_POLICY *policy
) {
    if (policy == NULL)
        return 0;

    return policy->state == POLICY_ACCEPTED;
}

const char *policy_state_name(
    POLICY_STATE state
) {
    switch (state) {
        case POLICY_UNKNOWN:
            return "UNKNOWN";

        case POLICY_INITIALIZING:
            return "INITIALIZING";

        case POLICY_VALIDATING:
            return "VALIDATING";

        case POLICY_ACCEPTED:
            return "ACCEPTED";

        case POLICY_REJECTED:
            return "REJECTED";

        default:
            return "INVALID";
    }
}

uint64_t policy_get_signature(
    const SECURITY_POLICY *policy
) {
    if (policy == NULL)
        return 0;

    return policy->policy_signature;
}