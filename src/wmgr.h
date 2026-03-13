/*
 * Window Manager - 9front-style window management
 * C89/C90 compliant
 *
 * Provides window list management and operations
 */

#ifndef WM_WMGR_H
#define WM_WMGR_H

/*
 * Forward declaration
 */
struct KryonWindow;

/*
 * Global window list
 * Note: These are defined in wmgr.c
 */
extern struct KryonWindow **g_windows;
extern int g_window_count;
extern int g_window_capacity;

extern struct KryonWindow **g_hidden_windows;
extern int g_hidden_count;
extern int g_hidden_capacity;

/*
 * Initialize window manager
 *
 * Returns 0 on success, -1 on error
 */
int wmgr_init(void);

/*
 * Cleanup window manager
 */
void wmgr_cleanup(void);

/*
 * Add window to list
 *
 * Parameters:
 *   win - Window to add
 *
 * Returns 0 on success, -1 on error
 */
int wmgr_add_window(struct KryonWindow *win);

/*
 * Remove window from list
 *
 * Parameters:
 *   win - Window to remove
 *
 * Returns 0 on success, -1 on error
 */
int wmgr_remove_window(struct KryonWindow *win);

/*
 * Get window by index
 *
 * Parameters:
 *   index - Window index (0-based)
 *
 * Returns window pointer, or NULL if not found
 */
struct KryonWindow *wmgr_get_window(int index);

/*
 * Get number of windows
 *
 * Returns count of visible windows
 */
int wmgr_get_window_count(void);

/*
 * Hide window
 *
 * Parameters:
 *   win - Window to hide
 *
 * Returns 0 on success, -1 on error
 */
int wmgr_hide_window(struct KryonWindow *win);

/*
 * Show (unhide) window
 *
 * Parameters:
 *   win - Window to show
 *
 * Returns 0 on success, -1 on error
 */
int wmgr_show_window(struct KryonWindow *win);

/*
 * Get hidden window by index
 *
 * Parameters:
 *   index - Hidden window index (0-based)
 *
 * Returns window pointer, or NULL if not found
 */
struct KryonWindow *wmgr_get_hidden_window(int index);

/*
 * Get number of hidden windows
 *
 * Returns count of hidden windows
 */
int wmgr_get_hidden_count(void);

/*
 * Get topmost window at screen position
 *
 * Parameters:
 *   x, y - Screen position
 *
 * Returns window pointer, or NULL if no window at position
 */
struct KryonWindow *wmgr_window_at_point(int x, int y);

/*
 * Bring window to front
 *
 * Parameters:
 *   win - Window to raise
 *
 * Returns 0 on success, -1 on error
 */
int wmgr_raise_window(struct KryonWindow *win);

/*
 * Delete window
 *
 * Parameters:
 *   win - Window to delete
 *
 * Returns 0 on success, -1 on error
 */
int wmgr_delete_window(struct KryonWindow *win);

/*
 * Create new window with shell
 *
 * Parameters:
 *   x, y - Window position
 *   width, height - Window dimensions
 *   with_shell - 1 to spawn shell, 0 otherwise
 *
 * Returns window pointer, or NULL on error
 */
struct KryonWindow *wmgr_create_window(int x, int y, int width, int height, int with_shell);

#endif /* WM_WMGR_H */
