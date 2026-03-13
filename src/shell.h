/*
 * Shell Spawning - Create shell processes in windows
 * C89/C90 compliant
 *
 * Provides functions to spawn rc shells in Kryon windows
 */

#ifndef WM_SHELL_H
#define WM_SHELL_H

/*
 * Forward declaration
 */
struct KryonWindow;

/*
 * Spawn rc shell in a window
 *
 * Parameters:
 *   win - Window to spawn shell in
 *   shell_cmd - Shell command (e.g., "rc") or NULL for default
 *
 * Returns 0 on success, -1 on error
 */
int shell_spawn_in_window(struct KryonWindow *win, const char *shell_cmd);

/*
 * Setup environment for shell
 *
 * Parameters:
 *   win - Window to setup environment for
 *
 * Returns 0 on success, -1 on error
 */
int shell_setup_environment(struct KryonWindow *win);

/*
 * Connect shell I/O to window
 *
 * Creates pipes for stdin/stdout and connects them to window's console
 *
 * Parameters:
 *   win - Window to connect I/O for
 *   shell_stdin - Output pipe for shell stdin
 *   shell_stdout - Input pipe for shell stdout
 *
 * Returns 0 on success, -1 on error
 */
int shell_connect_io(struct KryonWindow *win, int *shell_stdin, int *shell_stdout);

/*
 * Get default shell command
 *
 * Returns path to rc shell (or NULL if not found)
 */
const char *shell_get_default(void);

#endif /* WM_SHELL_H */
