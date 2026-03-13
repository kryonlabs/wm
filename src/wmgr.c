/*
 * Window Manager Implementation
 * C89/C90 compliant
 */

#include "wmgr.h"
#include "window.h"
#include "shell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Initial capacity for window arrays
 */
#define INITIAL_WINDOW_CAPACITY  16

/*
 * Global window lists
 */
struct KryonWindow **g_windows = NULL;
int g_window_count = 0;
int g_window_capacity = 0;

struct KryonWindow **g_hidden_windows = NULL;
int g_hidden_count = 0;
int g_hidden_capacity = 0;

/*
 * Initialize window manager
 */
int wmgr_init(void)
{
    /* Allocate window array */
    g_window_capacity = INITIAL_WINDOW_CAPACITY;
    g_windows = (struct KryonWindow **)malloc(
        sizeof(struct KryonWindow *) * g_window_capacity);
    if (g_windows == NULL) {
        return -1;
    }
    g_window_count = 0;

    /* Allocate hidden window array */
    g_hidden_capacity = INITIAL_WINDOW_CAPACITY;
    g_hidden_windows = (struct KryonWindow **)malloc(
        sizeof(struct KryonWindow *) * g_hidden_capacity);
    if (g_hidden_windows == NULL) {
        free(g_windows);
        g_windows = NULL;
        return -1;
    }
    g_hidden_count = 0;

    return 0;
}

/*
 * Cleanup window manager
 */
void wmgr_cleanup(void)
{
    int i;

    /* Destroy all visible windows */
    for (i = 0; i < g_window_count; i++) {
        if (g_windows[i] != NULL) {
            window_destroy(g_windows[i]);
            g_windows[i] = NULL;
        }
    }

    /* Destroy all hidden windows */
    for (i = 0; i < g_hidden_count; i++) {
        if (g_hidden_windows[i] != NULL) {
            window_destroy(g_hidden_windows[i]);
            g_hidden_windows[i] = NULL;
        }
    }

    /* Free arrays */
    if (g_windows != NULL) {
        free(g_windows);
        g_windows = NULL;
    }
    g_window_count = 0;
    g_window_capacity = 0;

    if (g_hidden_windows != NULL) {
        free(g_hidden_windows);
        g_hidden_windows = NULL;
    }
    g_hidden_count = 0;
    g_hidden_capacity = 0;
}

/*
 * Add window to list
 */
int wmgr_add_window(struct KryonWindow *win)
{
    if (win == NULL) {
        return -1;
    }

    /* Check if we need to expand array */
    if (g_window_count >= g_window_capacity) {
        int new_capacity;
        struct KryonWindow **new_array;

        new_capacity = g_window_capacity * 2;
        new_array = (struct KryonWindow **)realloc(
            g_windows, sizeof(struct KryonWindow *) * new_capacity);
        if (new_array == NULL) {
            return -1;
        }

        g_windows = new_array;
        g_window_capacity = new_capacity;
    }

    /* Add window to end of array */
    g_windows[g_window_count] = win;
    g_window_count++;

    return 0;
}

/*
 * Remove window from list
 */
int wmgr_remove_window(struct KryonWindow *win)
{
    int i;
    int found;

    if (win == NULL) {
        return -1;
    }

    /* Find window in visible list */
    found = 0;
    for (i = 0; i < g_window_count; i++) {
        if (g_windows[i] == win) {
            found = 1;
            /* Shift remaining windows down */
            for (; i < g_window_count - 1; i++) {
                g_windows[i] = g_windows[i + 1];
            }
            g_window_count--;
            break;
        }
    }

    /* Check hidden list if not found in visible */
    if (!found) {
        for (i = 0; i < g_hidden_count; i++) {
            if (g_hidden_windows[i] == win) {
                /* Shift remaining windows down */
                for (; i < g_hidden_count - 1; i++) {
                    g_hidden_windows[i] = g_hidden_windows[i + 1];
                }
                g_hidden_count--;
                return 0;
            }
        }
        return -1;
    }

    return 0;
}

/*
 * Get window by index
 */
struct KryonWindow *wmgr_get_window(int index)
{
    if (index < 0 || index >= g_window_count) {
        return NULL;
    }
    return g_windows[index];
}

/*
 * Get number of windows
 */
int wmgr_get_window_count(void)
{
    return g_window_count;
}

/*
 * Hide window
 */
int wmgr_hide_window(struct KryonWindow *win)
{
    int i;

    if (win == NULL) {
        return -1;
    }

    /* Find window in visible list */
    for (i = 0; i < g_window_count; i++) {
        if (g_windows[i] == win) {
            /* Remove from visible list */
            for (; i < g_window_count - 1; i++) {
                g_windows[i] = g_windows[i + 1];
            }
            g_window_count--;

            /* Add to hidden list */
            if (g_hidden_count >= g_hidden_capacity) {
                int new_capacity;
                struct KryonWindow **new_array;

                new_capacity = g_hidden_capacity * 2;
                new_array = (struct KryonWindow **)realloc(
                    g_hidden_windows, sizeof(struct KryonWindow *) * new_capacity);
                if (new_array == NULL) {
                    return -1;
                }

                g_hidden_windows = new_array;
                g_hidden_capacity = new_capacity;
            }

            g_hidden_windows[g_hidden_count] = win;
            g_hidden_count++;

            /* Mark window as not visible */
            win->visible = 0;

            return 0;
        }
    }

    return -1;
}

/*
 * Show (unhide) window
 */
int wmgr_show_window(struct KryonWindow *win)
{
    int i;

    if (win == NULL) {
        return -1;
    }

    /* Find window in hidden list */
    for (i = 0; i < g_hidden_count; i++) {
        if (g_hidden_windows[i] == win) {
            /* Remove from hidden list */
            for (; i < g_hidden_count - 1; i++) {
                g_hidden_windows[i] = g_hidden_windows[i + 1];
            }
            g_hidden_count--;

            /* Add to visible list */
            if (g_window_count >= g_window_capacity) {
                int new_capacity;
                struct KryonWindow **new_array;

                new_capacity = g_window_capacity * 2;
                new_array = (struct KryonWindow **)realloc(
                    g_windows, sizeof(struct KryonWindow *) * new_capacity);
                if (new_array == NULL) {
                    return -1;
                }

                g_windows = new_array;
                g_window_capacity = new_capacity;
            }

            g_windows[g_window_count] = win;
            g_window_count++;

            /* Mark window as visible */
            win->visible = 1;

            return 0;
        }
    }

    return -1;
}

/*
 * Get hidden window by index
 */
struct KryonWindow *wmgr_get_hidden_window(int index)
{
    if (index < 0 || index >= g_hidden_count) {
        return NULL;
    }
    return g_hidden_windows[index];
}

/*
 * Get number of hidden windows
 */
int wmgr_get_hidden_count(void)
{
    return g_hidden_count;
}

/*
 * Get topmost window at screen position
 */
struct KryonWindow *wmgr_window_at_point(int x, int y)
{
    int i;

    /* Search from top (last) to bottom */
    for (i = g_window_count - 1; i >= 0; i--) {
        struct KryonWindow *win = g_windows[i];
        int win_x, win_y, win_w, win_h;

        if (win == NULL || !win->visible) {
            continue;
        }

        /* Parse window rect */
        if (win->rect != NULL) {
            if (sscanf(win->rect, "%d %d %d %d", &win_x, &win_y, &win_w, &win_h) >= 4) {
                /* Check if point is in window */
                if (x >= win_x && x < win_x + win_w &&
                    y >= win_y && y < win_y + win_h) {
                    return win;
                }
            }
        }
    }

    return NULL;
}

/*
 * Bring window to front
 */
int wmgr_raise_window(struct KryonWindow *win)
{
    int i;
    int found;

    if (win == NULL) {
        return -1;
    }

    /* Find window */
    found = 0;
    for (i = 0; i < g_window_count; i++) {
        if (g_windows[i] == win) {
            found = 1;

            /* Move to end of array (top) */
            for (; i < g_window_count - 1; i++) {
                g_windows[i] = g_windows[i + 1];
            }
            g_windows[g_window_count - 1] = win;

            break;
        }
    }

    return found ? 0 : -1;
}

/*
 * Delete window
 */
int wmgr_delete_window(struct KryonWindow *win)
{
    if (win == NULL) {
        return -1;
    }

    /* Remove from list */
    if (wmgr_remove_window(win) < 0) {
        return -1;
    }

    /* Destroy window */
    window_destroy(win);

    return 0;
}

/*
 * Create new window with optional shell
 */
struct KryonWindow *wmgr_create_window(int x, int y, int width, int height, int with_shell)
{
    struct KryonWindow *win;
    char rect_str[64];
    int next_id;

    if (width <= 0 || height <= 0) {
        return NULL;
    }

    /* Get next window ID */
    next_id = g_window_count + 1;

    /* Create rect string */
    snprintf(rect_str, sizeof(rect_str), "%d %d %d %d", x, y, width, height);

    /* Create window */
    win = window_create("Shell Window", width, height);
    if (win == NULL) {
        return NULL;
    }

    /* Set window rect */
    if (window_set_rect(win, rect_str) < 0) {
        window_destroy(win);
        return NULL;
    }

    /* Set window visible */
    if (window_set_visible(win, 1) < 0) {
        window_destroy(win);
        return NULL;
    }

    /* Add to window list */
    if (wmgr_add_window(win) < 0) {
        window_destroy(win);
        return NULL;
    }

    /* Spawn shell if requested */
    if (with_shell) {
        if (shell_spawn_in_window(win, NULL) < 0) {
            fprintf(stderr, "Warning: failed to spawn shell in window %u\n", win->id);
            /* Continue anyway - window is still valid */
        }
    }

    fprintf(stderr, "Created new window %u at %d,%d (%dx%d)\n",
            win->id, x, y, width, height);

    return win;
}
