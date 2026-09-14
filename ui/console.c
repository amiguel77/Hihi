#include "console.h"
#include <stdio.h>

static HANDLE console_handle = NULL;

void console_initialize(void) {
    console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    console_set_default_color();
    console_set_cursor_visible(1);
}

void console_clear(void) {
    if (console_handle == NULL)
        return;

    CONSOLE_SCREEN_BUFFER_INFO info;

    if (!GetConsoleScreenBufferInfo(
            console_handle,
            &info))
        return;

    DWORD cells =
        (DWORD)info.dwSize.X *
        (DWORD)info.dwSize.Y;

    DWORD written;

    COORD origin = {0, 0};

    FillConsoleOutputCharacterA(
        console_handle,
        ' ',
        cells,
        origin,
        &written
    );

    FillConsoleOutputAttribute(
        console_handle,
        info.wAttributes,
        cells,
        origin,
        &written
    );

    SetConsoleCursorPosition(
        console_handle,
        origin
    );
}

void console_set_default_color(void) {
    if (console_handle != NULL)
        SetConsoleTextAttribute(
            console_handle,
            FOREGROUND_RED |
            FOREGROUND_GREEN |
            FOREGROUND_BLUE
        );
}

void console_set_red(void) {
    if (console_handle != NULL)
        SetConsoleTextAttribute(
            console_handle,
            FOREGROUND_RED |
            FOREGROUND_INTENSITY
        );
}

void console_set_green(void) {
    if (console_handle != NULL)
        SetConsoleTextAttribute(
            console_handle,
            FOREGROUND_GREEN |
            FOREGROUND_INTENSITY
        );
}

void console_set_yellow(void) {
    if (console_handle != NULL)
        SetConsoleTextAttribute(
            console_handle,
            FOREGROUND_RED |
            FOREGROUND_GREEN |
            FOREGROUND_INTENSITY
        );
}

void console_set_gray(void) {
    if (console_handle != NULL)
        SetConsoleTextAttribute(
            console_handle,
            FOREGROUND_INTENSITY
        );
}

void console_write(const char *text) {
    if (text == NULL)
        return;

    printf("%s", text);
    fflush(stdout);
}

void console_write_line(const char *text) {
    if (text == NULL)
        return;

    printf("%s\n", text);
    fflush(stdout);
}

void console_write_red(const char *text) {
    console_set_red();
    console_write(text);
    console_set_default_color();
}

void console_write_red_line(const char *text) {
    console_set_red();
    console_write_line(text);
    console_set_default_color();
}

void console_set_cursor_visible(int visible) {
    if (console_handle == NULL)
        return;

    CONSOLE_CURSOR_INFO cursor;

    if (!GetConsoleCursorInfo(
            console_handle,
            &cursor))
        return;

    cursor.bVisible = visible ? TRUE : FALSE;

    SetConsoleCursorInfo(
        console_handle,
        &cursor
    );
}

void console_sleep(unsigned int milliseconds) {
    Sleep(milliseconds);
}