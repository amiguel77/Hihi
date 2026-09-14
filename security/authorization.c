#include "authorization.h"
#include <windows.h>

static uint64_t generate_session_id(void) {
    uint64_t value = (uint64_t)GetCurrentProcessId();

    value ^= 0xD6E8FEB86659FD93ULL;
    value *= 0x9E3779B185EBCA87ULL;
    value ^= value >> 29;

    return value;
}

void authorization_initialize(
    AUTHORIZATION_SESSION *session
) {
    if (session == NULL)
        return;

    ZeroMemory(
        session,
        sizeof(AUTHORIZATION_SESSION)
    );

    session->state = AUTH_UNINITIALIZED;
    session->reason = AUTH_REASON_NONE;
    session->session_id = generate_session_id();

    integrity_initialize(
        &session->integrity
    );

    policy_initialize(
        &session->policy
    );

    session->state = AUTH_CONTEXT_RESOLUTION;
}

int authorization_resolve_context(
    AUTHORIZATION_SESSION *session
) {
    if (session == NULL)
        return 0;

    if (session->state != AUTH_CONTEXT_RESOLUTION) {
        session->reason = AUTH_REASON_CONTEXT_INVALID;
        session->state = AUTH_REJECTED;
        return 0;
    }

    if (session->integrity.process_id == 0) {
        session->reason = AUTH_REASON_CONTEXT_INVALID;
        session->state = AUTH_REJECTED;
        return 0;
    }

    session->state = AUTH_INTEGRITY_VALIDATION;

    return 1;
}

int authorization_validate_integrity(
    AUTHORIZATION_SESSION *session
) {
    if (session == NULL)
        return 0;

    if (session->state != AUTH_INTEGRITY_VALIDATION) {
        session->reason = AUTH_REASON_INTEGRITY_FAILURE;
        session->state = AUTH_REJECTED;
        return 0;
    }

    if (!integrity_validate(
            &session->integrity)) {

        session->reason =
            AUTH_REASON_INTEGRITY_FAILURE;

        session->state = AUTH_REJECTED;

        return 0;
    }

    session->state = AUTH_POLICY_VALIDATION;

    return 1;
}

int authorization_validate_policy(
    AUTHORIZATION_SESSION *session
) {
    if (session == NULL)
        return 0;

    if (session->state != AUTH_POLICY_VALIDATION) {
        session->reason = AUTH_REASON_POLICY_FAILURE;
        session->state = AUTH_REJECTED;
        return 0;
    }

    if (!policy_validate(
            &session->policy,
            &session->integrity)) {

        session->reason =
            AUTH_REASON_POLICY_FAILURE;

        session->state = AUTH_REJECTED;

        return 0;
    }

    session->authorization_key =
        policy_get_signature(
            &session->policy
        );

    session->state = AUTH_OVERRIDE_PENDING;

    return 1;
}

int authorization_request_override(
    AUTHORIZATION_SESSION *session
) {
    if (session == NULL)
        return 0;

    if (session->state != AUTH_OVERRIDE_PENDING) {
        session->reason =
            AUTH_REASON_HOST_PROTECTED;

        session->state = AUTH_REJECTED;

        return 0;
    }

    if (!integrity_is_protected(
            &session->integrity)) {

        session->reason =
            AUTH_REASON_HOST_PROTECTED;

        session->state = AUTH_REJECTED;

        return 0;
    }

    session->state = AUTH_COMMITTED;

    return 1;
}

int authorization_commit(
    AUTHORIZATION_SESSION *session
) {
    if (session == NULL)
        return 0;

    if (session->state != AUTH_COMMITTED) {
        session->reason =
            AUTH_REASON_COMMIT_FAILURE;

        session->state = AUTH_REJECTED;

        return 0;
    }

    return 1;
}

int authorization_execute(
    AUTHORIZATION_SESSION *session
) {
    if (session == NULL)
        return 0;

    authorization_initialize(session);

    if (!authorization_resolve_context(session))
        return 0;

    if (!authorization_validate_integrity(session))
        return 0;

    if (!authorization_validate_policy(session))
        return 0;

    if (!authorization_request_override(session))
        return 0;

    if (!authorization_commit(session))
        return 0;

    return 1;
}

const char *authorization_state_name(
    AUTH_STATE state
) {
    switch (state) {
        case AUTH_UNINITIALIZED:
            return "UNINITIALIZED";

        case AUTH_CONTEXT_RESOLUTION:
            return "CONTEXT_RESOLUTION";

        case AUTH_INTEGRITY_VALIDATION:
            return "INTEGRITY_VALIDATION";

        case AUTH_POLICY_VALIDATION:
            return "POLICY_VALIDATION";

        case AUTH_OVERRIDE_PENDING:
            return "OVERRIDE_PENDING";

        case AUTH_COMMITTED:
            return "COMMITTED";

        case AUTH_REJECTED:
            return "REJECTED";

        default:
            return "UNKNOWN";
    }
}

const char *authorization_reason_name(
    AUTH_REASON reason
) {
    switch (reason) {
        case AUTH_REASON_NONE:
            return "NONE";

        case AUTH_REASON_CONTEXT_INVALID:
            return "CONTEXT_INVALID";

        case AUTH_REASON_INTEGRITY_FAILURE:
            return "INTEGRITY_FAILURE";

        case AUTH_REASON_POLICY_FAILURE:
            return "POLICY_FAILURE";

        case AUTH_REASON_HOST_PROTECTED:
            return "HOST_PROTECTED";

        case AUTH_REASON_COMMIT_FAILURE:
            return "COMMIT_FAILURE";

        default:
            return "UNKNOWN";
    }
}