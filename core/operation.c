#include "operation.h"

#include "../ui/console.h"
#include "../ui/progress.h"

#include <stdio.h>
#include <stdlib.h>

static void print_component_header(
    const SYSTEM_COMPONENT *component,
    size_t index,
    size_t total
) {
    console_set_red();

    printf(
        "\n[%03zu/%03zu] %s\n",
        index + 1,
        total,
        component->name
    );

    printf(
        " TYPE        : %s\n",
        component_type_name(component->type)
    );

    printf(
        " PRIORITY    : %s\n",
        component_priority_name(component->priority)
    );

    printf(
        " SIZE        : %llu MB\n",
        component->size_mb
    );

    printf(
        " DEPENDENCIES: %u\n",
        component->dependencies
    );

    printf(
        " STATE       : %s\n",
        component_state_name(component->state)
    );

    console_set_default_color();
}

static unsigned int calculate_component_delay(
    const SYSTEM_COMPONENT *component
) {
    if (component == NULL)
        return 100;
    
    unsigned int base = 45;

    if (component->size_mb < 16) {
        base += 20;
    } else if (component->size_mb < 128) {
        base += 35;
    } else if (component->size_mb < 512) {
        base += 55;
    } else if (component->size_mb < 2048) {
        base += 80;
    } else if (component->size_mb < 8192) {
        base += 120;
    } else {
        base += 180;
    }

    base += component->dependencies * 2;

    base += rand() % 40;

    return base;
}

static void process_component(
    OPERATION_CONTEXT *operation,
    SYSTEM_COMPONENT *component,
    size_t index
) {
    if (operation == NULL ||
        component == NULL)
        return;

    size_t total =
        inventory_count(
            &operation->session.inventory
        );

    print_component_header(
        component,
        index,
        total
    );

    component_set_state(
        component,
        COMPONENT_LOCKED
    );

    console_set_red();
    console_write_line(
        " STATE       : LOCKED"
    );

    component_set_state(
        component,
        COMPONENT_PROCESSING
    );

    console_write_red_line(
        " RESOLVING   : dependency graph"
    );

    console_write_red_line(
        " VALIDATING  : component metadata"
    );

    console_write_red_line(
        " PREPARING   : allocation context"
    );

    PROGRESS_STATE progress;

    progress_initialize(
        &progress,
        " PROCESSING",
        42
    );

    unsigned int steps = 20;

    for (unsigned int i = 1;
         i <= steps;
         i++) {

        unsigned int percent =
            i * 100 / steps;

        progress_set(
            &progress,
            percent
        );

        progress_render(
            &progress
        );

        console_sleep(
            calculate_component_delay(
                component
            )
        );
    }

    progress_complete(&progress);

    allocation_process(
        &operation->session.allocation,
        component->size_mb * 1024
    );

    filesystem_process(
        &operation->session.filesystem,
        component->size_mb * 1024
    );

    operation->processed_mb +=
        component->size_mb;

    operation->current_size_mb =
        component->size_mb;

    component_set_state(
        component,
        COMPONENT_RELEASED
    );

    console_write_red_line(
        " RELEASE    : component state committed"
    );

    component_set_state(
        component,
        COMPONENT_COMPLETED
    );

    operation->session.completed_components++;

    console_write_red_line(
        " RESULT     : COMPLETED"
    );
}

void operation_initialize(
    OPERATION_CONTEXT *operation
) {
    if (operation == NULL)
        return;

    ZeroMemory(
        operation,
        sizeof(OPERATION_CONTEXT)
    );

    operation->state =
        OPERATION_INITIALIZING;

    session_initialize(
        &operation->session
    );

    operation->current_percent = 0;
    operation->component_percent = 0;
    operation->current_size_mb = 0;
    operation->processed_mb = 0;

    operation->authorization_granted = 0;
    operation->host_protection_triggered = 0;
}

int operation_authorize(
    OPERATION_CONTEXT *operation
) {
    if (operation == NULL)
        return 0;

    if (operation->state !=
        OPERATION_INITIALIZING)
        return 0;

    operation->state =
        OPERATION_AUTHORIZING;

    session_start(
        &operation->session
    );

    console_set_red();
    console_write_line(
        "\nAUTHORIZATION CONTEXT"
    );

    console_write_red_line(
        " Resolving process context..."
    );

    console_sleep(350);

    console_write_red_line(
        " Validating integrity boundary..."
    );

    console_sleep(350);

    if (!authorization_resolve_context(
            &operation->session.authorization)) {

        operation_abort(operation);
        return 0;
    }

    if (!authorization_validate_integrity(
            &operation->session.authorization)) {

        operation_abort(operation);
        return 0;
    }

    console_write_red_line(
        " Validating security policy..."
    );

    console_sleep(350);

    if (!authorization_validate_policy(
            &operation->session.authorization)) {

        operation_abort(operation);
        return 0;
    }

    console_write_red_line(
        " Authorization context accepted."
    );

    operation->authorization_granted = 1;

    /*
     * The internal authorization chain reaches
     * the protected-host boundary here.
     */
    if (!authorization_request_override(
            &operation->session.authorization)) {

        operation->host_protection_triggered = 1;

        console_write_red_line(
            " Host integrity boundary rejected override."
        );

        operation_abort(operation);
        return 0;
    }

    return 1;
}

void operation_scan(
    OPERATION_CONTEXT *operation
) {
    if (operation == NULL)
        return;

    if (!operation->authorization_granted)
        return;

    if (operation->state !=
        OPERATION_AUTHORIZING)
        return;

    operation->state =
        OPERATION_SCANNING;

    session_prepare(
        &operation->session
    );

    console_set_red();

    console_write_line(
        "\nSYSTEM COMPONENT INVENTORY"
    );

    printf(
        " Components detected : %zu\n",
        inventory_count(
            &operation->session.inventory
        )
    );

    printf(
        " Aggregate size      : %llu MB\n",
        inventory_total_size(
            &operation->session.inventory
        )
    );

    printf(
        " Dependency entries  : %u\n",
        inventory_total_dependencies(
            &operation->session.inventory
        )
    );

    console_set_default_color();

    console_sleep(500);
}

void operation_process(
    OPERATION_CONTEXT *operation
) {
    if (operation == NULL)
        return;

    if (operation->state !=
        OPERATION_SCANNING)
        return;

    operation->state =
        OPERATION_PROCESSING;

    session_begin(
        &operation->session
    );

    size_t total =
        inventory_count(
            &operation->session.inventory
        );

    for (size_t i = 0;
         i < total;
         i++) {

        SYSTEM_COMPONENT *component =
            inventory_get(
                &operation->session.inventory,
                i
            );

        if (component == NULL)
            continue;

        operation->session.current_component =
            i;

        process_component(
            operation,
            component,
            i
        );

        operation->current_percent =
            (unsigned int)(
                (i + 1) * 100 / total
            );
    }
}

void operation_finalize(
    OPERATION_CONTEXT *operation
) {
    if (operation == NULL)
        return;

    if (operation->state !=
        OPERATION_PROCESSING)
        return;

    operation->state =
        OPERATION_FINALIZING;

    session_finalize(
        &operation->session
    );

    console_set_red();

    console_write_line(
        "\nFINALIZATION"
    );

    console_write_red_line(
        " Synchronizing operation state..."
    );

    console_sleep(400);

    console_write_red_line(
        " Flushing allocation context..."
    );

    console_sleep(400);

    console_write_red_line(
        " Validating host boundary..."
    );

    console_sleep(500);

    /*
     * The protected execution environment is
     * deliberately the final authority.
     */
    operation->host_protection_triggered = 1;

    console_write_red_line(
        " HOST PROTECTION: ACTIVE"
    );

    console_write_red_line(
        " Destructive I/O request rejected."
    );

    operation_abort(operation);
}

void operation_abort(
    OPERATION_CONTEXT *operation
) {
    if (operation == NULL)
        return;

    operation->state =
        OPERATION_ABORTING;

    session_abort(
        &operation->session
    );

    operation->state =
        OPERATION_TERMINATED;
}

void operation_run(
    OPERATION_CONTEXT *operation
) {
    if (operation == NULL)
        return;

    if (!operation_authorize(operation))
        return;

    operation_scan(operation);

    if (operation->state !=
        OPERATION_SCANNING)
        return;

    operation_process(operation);

    if (operation->state !=
        OPERATION_PROCESSING)
        return;

    operation_finalize(operation);
}

int operation_is_terminated(
    const OPERATION_CONTEXT *operation
) {
    if (operation == NULL)
        return 0;

    return operation->state ==
           OPERATION_TERMINATED;
}

const char *operation_state_name(
    OPERATION_STATE state
) {
    switch (state) {
        case OPERATION_IDLE:
            return "IDLE";

        case OPERATION_INITIALIZING:
            return "INITIALIZING";

        case OPERATION_AUTHORIZING:
            return "AUTHORIZING";

        case OPERATION_SCANNING:
            return "SCANNING";

        case OPERATION_PROCESSING:
            return "PROCESSING";

        case OPERATION_FINALIZING:
            return "FINALIZING";

        case OPERATION_ABORTING:
            return "ABORTING";

        case OPERATION_TERMINATED:
            return "TERMINATED";

        case OPERATION_COMPLETED:
            return "COMPLETED";

        default:
            return "UNKNOWN";
    }
}