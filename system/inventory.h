#ifndef INVENTORY_H
#define INVENTORY_H

#include "components.h"

#define MAX_COMPONENTS 64

typedef struct {
    SYSTEM_COMPONENT components[MAX_COMPONENTS];
    size_t count;
    unsigned long long total_size_mb;
    unsigned int total_dependencies;
} SYSTEM_INVENTORY;

void inventory_initialize(
    SYSTEM_INVENTORY *inventory
);

int inventory_add(
    SYSTEM_INVENTORY *inventory,
    const char *name,
    COMPONENT_TYPE type,
    COMPONENT_PRIORITY priority,
    unsigned long long size_mb,
    unsigned int dependencies
);

SYSTEM_COMPONENT *inventory_get(
    SYSTEM_INVENTORY *inventory,
    size_t index
);

size_t inventory_count(
    const SYSTEM_INVENTORY *inventory
);

unsigned long long inventory_total_size(
    const SYSTEM_INVENTORY *inventory
);

unsigned int inventory_total_dependencies(
    const SYSTEM_INVENTORY *inventory
);

void inventory_scan(
    SYSTEM_INVENTORY *inventory
);

#endif