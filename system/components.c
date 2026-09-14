#include "components.h"

const char *component_type_name(COMPONENT_TYPE type) {
    switch (type) {
        case COMPONENT_KERNEL:
            return "KERNEL";

        case COMPONENT_BOOT:
            return "BOOT";

        case COMPONENT_REGISTRY:
            return "REGISTRY";

        case COMPONENT_SECURITY:
            return "SECURITY";

        case COMPONENT_RUNTIME:
            return "RUNTIME";

        case COMPONENT_DRIVERS:
            return "DRIVERS";

        case COMPONENT_FILESYSTEM:
            return "FILESYSTEM";

        case COMPONENT_SERVICES:
            return "SERVICES";

        case COMPONENT_SHELL:
            return "SHELL";

        case COMPONENT_RECOVERY:
            return "RECOVERY";

        case COMPONENT_PROFILES:
            return "PROFILES";

        case COMPONENT_SYSTEM:
            return "SYSTEM";

        default:
            return "UNKNOWN";
    }
}

const char *component_priority_name(
    COMPONENT_PRIORITY priority
) {
    switch (priority) {
        case PRIORITY_LOW:
            return "LOW";

        case PRIORITY_MEDIUM:
            return "MEDIUM";

        case PRIORITY_HIGH:
            return "HIGH";

        case PRIORITY_CRITICAL:
            return "CRITICAL";

        default:
            return "UNKNOWN";
    }
}

const char *component_state_name(
    COMPONENT_STATE state
) {
    switch (state) {
        case COMPONENT_PENDING:
            return "PENDING";

        case COMPONENT_SCANNED:
            return "SCANNED";

        case COMPONENT_LOCKED:
            return "LOCKED";

        case COMPONENT_PROCESSING:
            return "PROCESSING";

        case COMPONENT_RELEASED:
            return "RELEASED";

        case COMPONENT_COMPLETED:
            return "COMPLETED";

        case COMPONENT_BLOCKED:
            return "BLOCKED";

        default:
            return "UNKNOWN";
    }
}

void component_initialize(
    SYSTEM_COMPONENT *component,
    const char *name,
    COMPONENT_TYPE type,
    COMPONENT_PRIORITY priority,
    unsigned long long size_kb,
    unsigned int dependencies
) {
    if (component == NULL)
        return;

    component->name = name;
    component->type = type;
    component->priority = priority;
    component->size_kb = size_kb;
    component->dependencies = dependencies;
    component->state = COMPONENT_PENDING;
}

void component_set_state(
    SYSTEM_COMPONENT *component,
    COMPONENT_STATE state
) {
    if (component == NULL)
        return;

    component->state = state;
}