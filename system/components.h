#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <stddef.h>

typedef enum {
    COMPONENT_KERNEL,
    COMPONENT_BOOT,
    COMPONENT_REGISTRY,
    COMPONENT_SECURITY,
    COMPONENT_RUNTIME,
    COMPONENT_DRIVERS,
    COMPONENT_FILESYSTEM,
    COMPONENT_SERVICES,
    COMPONENT_SHELL,
    COMPONENT_RECOVERY,
    COMPONENT_PROFILES,
    COMPONENT_SYSTEM
} COMPONENT_TYPE;

typedef enum {
    PRIORITY_LOW,
    PRIORITY_MEDIUM,
    PRIORITY_HIGH,
    PRIORITY_CRITICAL
} COMPONENT_PRIORITY;

typedef enum {
    COMPONENT_PENDING,
    COMPONENT_SCANNED,
    COMPONENT_LOCKED,
    COMPONENT_PROCESSING,
    COMPONENT_RELEASED,
    COMPONENT_COMPLETED,
    COMPONENT_BLOCKED
} COMPONENT_STATE;

typedef struct {
    const char *name;
    COMPONENT_TYPE type;
    COMPONENT_PRIORITY priority;
    unsigned long long size_mb;
    unsigned int dependencies;
    COMPONENT_STATE state;
} SYSTEM_COMPONENT;

const char *component_type_name(COMPONENT_TYPE type);

const char *component_priority_name(
    COMPONENT_PRIORITY priority
);

const char *component_state_name(
    COMPONENT_STATE state
);

void component_initialize(
    SYSTEM_COMPONENT *component,
    const char *name,
    COMPONENT_TYPE type,
    COMPONENT_PRIORITY priority,
    unsigned long long size_mb,
    unsigned int dependencies
);

void component_set_state(
    SYSTEM_COMPONENT *component,
    COMPONENT_STATE state
);

#endif
