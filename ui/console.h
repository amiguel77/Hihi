#ifndef CONSOLE_H
#define CONSOLE_H

#include <windows.h>

void console_initialize(void);
void console_clear(void);

void console_set_default_color(void);
void console_set_red(void);
void console_set_green(void);
void console_set_yellow(void);
void console_set_gray(void);

void console_write(const char *text);
void console_write_line(const char *text);

void console_write_red(const char *text);
void console_write_red_line(const char *text);

void console_set_cursor_visible(int visible);

void console_sleep(unsigned int milliseconds);

#endif