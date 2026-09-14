#ifndef PROGRESS_H
#define PROGRESS_H

#include <stddef.h>

typedef struct {
    unsigned int percent;
    unsigned int width;
    const char *label;
} PROGRESS_STATE;

void progress_initialize(
    PROGRESS_STATE *progress,
    const char *label,
    unsigned int width
);

void progress_set(
    PROGRESS_STATE *progress,
    unsigned int percent
);

void progress_render(
    const PROGRESS_STATE *progress
);

void progress_complete(
    PROGRESS_STATE *progress
);

#endif
