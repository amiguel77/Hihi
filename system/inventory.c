#include "inventory.h"

void inventory_initialize(
    SYSTEM_INVENTORY *inventory
) {
    if (inventory == NULL)
        return;

    inventory->count = 0;
    inventory->total_size_kb = 0;
    inventory->total_dependencies = 0;

    for (size_t i = 0; i < MAX_COMPONENTS; i++) {
        inventory->components[i].name = NULL;
        inventory->components[i].type = COMPONENT_SYSTEM;
        inventory->components[i].priority = PRIORITY_LOW;
        inventory->components[i].size_kb = 0;
        inventory->components[i].dependencies = 0;
        inventory->components[i].state = COMPONENT_PENDING;
    }
}

int inventory_add(
    SYSTEM_INVENTORY *inventory,
    const char *name,
    COMPONENT_TYPE type,
    COMPONENT_PRIORITY priority,
    unsigned long long size_kb,
    unsigned int dependencies
) {
    if (inventory == NULL)
        return 0;

    if (name == NULL)
        return 0;

    if (inventory->count >= MAX_COMPONENTS)
        return 0;

    SYSTEM_COMPONENT *component =
        &inventory->components[inventory->count];

    component_initialize(
        component,
        name,
        type,
        priority,
        size_kb,
        dependencies
    );

    inventory->total_size_kb += size_kb;
    inventory->total_dependencies += dependencies;
    inventory->count++;

    return 1;
}

SYSTEM_COMPONENT *inventory_get(
    SYSTEM_INVENTORY *inventory,
    size_t index
) {
    if (inventory == NULL)
        return NULL;

    if (index >= inventory->count)
        return NULL;

    return &inventory->components[index];
}

size_t inventory_count(
    const SYSTEM_INVENTORY *inventory
) {
    if (inventory == NULL)
        return 0;

    return inventory->count;
}

unsigned long long inventory_total_size(
    const SYSTEM_INVENTORY *inventory
) {
    if (inventory == NULL)
        return 0;

    return inventory->total_size_kb;
}

unsigned int inventory_total_dependencies(
    const SYSTEM_INVENTORY *inventory
) {
    if (inventory == NULL)
        return 0;

    return inventory->total_dependencies;
}

void inventory_scan(
    SYSTEM_INVENTORY *inventory
) {
    if (inventory == NULL)
        return;

    for (size_t i = 0; i < inventory->count; i++) {
        SYSTEM_COMPONENT *component =
            &inventory->components[i];

        if (component->state == COMPONENT_PENDING) {
            component_set_state(
                component,
                COMPONENT_SCANNED
            );
        }
    }
}