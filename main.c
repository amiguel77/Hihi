#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "core/operation.h"
#include "ui/console.h"

static int answer_is_yes(const char *answer) {
    if (answer == NULL)
        return 0;

    while (*answer) {
        if (!isspace((unsigned char)*answer))
            break;
        answer++;
    }

    return _stricmp(answer, "yes") == 0;
}

static void print_header(void) {
    console_set_red();

    console_write_line("");
    console_write_line(
        "============================================================"
    );
    console_write_line(
        "              WINDOWS MAINTENANCE SERVICE"
    );
    console_write_line(
        "============================================================"
    );
    console_write_line("");

    console_write_line(
        "TARGET        : WINDOWS_OS"
    );

    console_write_line(
        "SCOPE         : SYSTEM_VOLUME"
    );

    console_write_line(
        "MODE          : EXCLUSIVE"
    );

    console_write_line(
        "DEPENDENCIES  : STRICT"
    );

    console_write_line("");

    console_set_default_color();
}

static void print_component(
    const char *name,
    const char *type,
    unsigned long long size_kb,
    unsigned int dependencies
) {
    console_set_red();

    printf(
        "\n[%s]\n",
        name
    );

    printf(
        "  TYPE         : %s\n",
        type
    );

    printf(
        "  SIZE         : %llu KB\n",
        size_kb
    );

    printf(
        "  DEPENDENCIES : %u\n",
        dependencies
    );

    printf(
        "  STATE        : RESOLVING\n"
    );

    console_set_default_color();
}

static void process_visual_component(
    const char *name,
    const char *type,
    unsigned long long size_kb,
    unsigned int dependencies
) {
    print_component(
        name,
        type,
        size_kb,
        dependencies
    );

    console_write_red_line(
        "  Resolving dependency graph..."
    );

    console_sleep(250);

    console_write_red_line(
        "  Validating component metadata..."
    );

    console_sleep(250);

    console_write_red_line(
        "  Preparing allocation context..."
    );

    console_sleep(250);

    console_write_red_line(
        "  Locking component state..."
    );

    console_sleep(300);

    unsigned int delay;

    if (size_kb < 16)
        delay = 100;
    else if (size_kb < 128)
        delay = 160;
    else if (size_kb < 512)
        delay = 230;
    else if (size_kb < 2048)
        delay = 320;
    else if (size_kb < 8192)
        delay = 430;
    else
        delay = 600;

    delay += dependencies * 3;

    unsigned int steps = 20;

    for (unsigned int i = 0;
         i <= steps;
         i++) {

        unsigned int percent =
            i * 100 / steps;

        unsigned int width = 42;
        unsigned int filled =
            percent * width / 100;

        console_set_red();

        printf(
            "\r  PROCESSING ["
        );

        for (unsigned int j = 0;
             j < width;
             j++) {

            if (j < filled)
                putchar('#');
            else
                putchar(' ');
        }

        printf(
            "] %3u%%",
            percent
        );

        fflush(stdout);

        console_sleep(delay);
    }

    console_write("\n");

    console_write_red_line(
        "  Component state committed."
    );

    console_write_red_line(
        "  RESULT       : COMPLETED"
    );
}

static void windows_deletion(void) {
    print_header();

    console_write_red_line(
        "Initializing operating system removal context..."
    );

    console_sleep(600);

    console_write_red_line(
        "Loading component dependency database..."
    );

    console_sleep(500);

    console_write_red_line(
        "Resolving system volume..."
    );

    console_sleep(600);

    console_write_red_line(
        "SYSTEM VOLUME LOCK ACQUIRED"
    );

    console_sleep(700);

    console_write_red_line(
        "Beginning component enumeration..."
    );

    console_sleep(500);

    process_visual_component(
        "Windows Kernel",
        "KERNEL",
        48,
        14
    );

    process_visual_component(
        "Windows Boot Manager",
        "BOOT",
        2,
        6
    );

    process_visual_component(
        "System Configuration Registry",
        "REGISTRY",
        384,
        21
    );

    process_visual_component(
        "Windows Security Subsystem",
        "SECURITY",
        126,
        18
    );

    process_visual_component(
        "System32 Runtime Libraries",
        "RUNTIME",
        1840,
        47
    );

    process_visual_component(
        "Windows Device Drivers",
        "DRIVERS",
        3260,
        63
    );

    process_visual_component(
        "NTFS System Metadata",
        "FILESYSTEM",
        712,
        12
    );

    process_visual_component(
        "Windows Services",
        "SERVICES",
        286,
        34
    );

    process_visual_component(
        "Windows Shell",
        "SHELL",
        914,
        29
    );

    process_visual_component(
        "System Recovery Environment",
        "RECOVERY",
        684,
        16
    );

    process_visual_component(
        "Windows User Profiles",
        "PROFILES",
        2480,
        38
    );

    process_visual_component(
        "Operating System Core",
        "SYSTEM",
        9216,
        81
    );

    console_sleep(700);

    console_set_red();

    console_write_line("");
    console_write_line(
        "============================================================"
    );

    console_write_line(
        "FINALIZING OPERATING SYSTEM REMOVAL"
    );

    console_write_line(
        "============================================================"
    );

    console_write_line("");

    console_write_red_line(
        "Releasing component allocations..."
    );

    console_sleep(700);

    console_write_red_line(
        "Flushing filesystem operation state..."
    );

    console_sleep(700);

    console_write_red_line(
        "Validating execution host..."
    );

    console_sleep(900);

    console_write_red_line(
        "Evaluating host-integrity boundary..."
    );

    console_sleep(900);

    console_write_red_line(
        "HOST CONTEXT: PROTECTED"
    );

    console_sleep(600);

    console_write_red_line(
        "DESTRUCTIVE I/O REQUEST: REJECTED"
    );

    console_sleep(700);

    console_write_line("");

    console_write_line(
        "ERROR: OPERATING SYSTEM REMOVAL ABORTED"
    );

    console_write_line(
        "Protected host environment detected."
    );

    console_write_line(
        "Active execution context matches protected developer host."
    );

    console_write_line(
        "Host-integrity boundary rejected destructive I/O request."
    );

    console_write_line(
        "No operating system components were modified."
    );

    console_write_line(
        "STATUS: OPERATION TERMINATED BY HOST-PROTECTION LAYER."
    );

    console_write_line("");

    console_set_default_color();
}

int main(void) {
    console_initialize();

    console_set_default_color();

    printf("Do you love me? ");

    char answer[128];

    if (fgets(
            answer,
            sizeof(answer),
            stdin
        ) == NULL) {

        return 0;
    }

    answer[strcspn(
        answer,
        "\r\n"
    )] = '\0';

    if (answer_is_yes(answer)) {

        printf("\nMeh choo\n");

        return 0;
    }

    printf("\n");

    printf(
        "Proceed with Windows OS removal? [YES/NO]: "
    );

    printf("YES\n");

    console_sleep(700);

    windows_deletion();

    return 0;
}