#include "progress.h"
#include "console.h"

#include <stdio.h>

void progress_initialize(
    PROGRESS_STATE *progress,
    const char *label,
    unsigned int width
) {
    if (progress == NULL)
        return;

    progress->percent = 0;
    progress->width = width;
    progress->label = label;
}

void progress_set(
    PROGRESS_STATE *progress,
    unsigned int percent
) {
    if (progress == NULL)
        return;

    if (percent > 100)
        percent = 100;

    progress->percent = percent;
}

void progress_render(
    const PROGRESS_STATE *progress
) {
    if (progress == NULL)
        return;

    unsigned int filled =
        progress->percent *
        progress->width / 100;

    console_set_red();

    printf(
        "\r%s [",
        progress->label != NULL
            ? progress->label
            : "PROCESS"
    );

    for (unsigned int i = 0;
         i < progress->width;
         i++) {

        if (i < filled)
            putchar('#');
        else
            putchar(' ');
    }

    printf(
        "] %3u%%",
        progress->percent
    );

    fflush(stdout);

    console_set_default_color();
}

void progress_complete(
    PROGRESS_STATE *progress
) {
    if (progress == NULL)
        return;

    progress_set(progress, 100);
    progress_render(progress);
    console_write("\n");
}