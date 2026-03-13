/*
 * Shell Spawning Implementation
 * C89/C90 compliant
 */

#include "shell.h"
#include "window.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

/*
 * Default shell paths to try
 */
static const char *shell_paths[] = {
    "./amd64/bin/rc",
    "/usr/local/bin/rc",
    "/usr/bin/rc",
    "/bin/rc",
    "rc",
    NULL
};

/*
 * Get default shell command
 */
const char *shell_get_default(void)
{
    int i;

    for (i = 0; shell_paths[i] != NULL; i++) {
        /* Check if file exists and is executable */
        if (access(shell_paths[i], X_OK) == 0) {
            return shell_paths[i];
        }
    }

    return NULL;
}

/*
 * Setup environment for shell
 */
int shell_setup_environment(struct KryonWindow *win)
{
    (void)win;

    /* Set up environment variables for the shell */
    /* These would typically be set in the child process after fork */

    /* TODO: Set up environment like:
     * - PATH
     * - HOME
     * - TERM
     * - window ID
     */

    return 0;
}

/*
 * Connect shell I/O to window
 */
int shell_connect_io(struct KryonWindow *win, int *shell_stdin, int *shell_stdout)
{
    int stdin_pipe[2];
    int stdout_pipe[2];

    if (win == NULL || shell_stdin == NULL || shell_stdout == NULL) {
        return -1;
    }

    /* Create pipes for stdin/stdout */
    if (pipe(stdin_pipe) < 0) {
        perror("pipe stdin");
        return -1;
    }

    if (pipe(stdout_pipe) < 0) {
        perror("pipe stdout");
        close(stdin_pipe[0]);
        close(stdin_pipe[1]);
        return -1;
    }

    /* Store pipe ends for caller */
    *shell_stdin = stdin_pipe[1];   /* Write end of stdin pipe */
    *shell_stdout = stdout_pipe[0]; /* Read end of stdout pipe */

    /* TODO: Connect pipes to window's console device */
    /* For now, we'll need to set up a mechanism to:
     * 1. Read from stdout_pipe[0] and write to window's console
     * 2. Read from window's console input and write to stdin_pipe[1]
     */

    return 0;
}

/*
 * Spawn rc shell in a window
 */
int shell_spawn_in_window(struct KryonWindow *win, const char *shell_cmd)
{
    pid_t pid;
    const char *cmd;
    int stdin_pipe[2];
    int stdout_pipe[2];

    if (win == NULL) {
        return -1;
    }

    /* Determine shell command */
    if (shell_cmd == NULL) {
        cmd = shell_get_default();
        if (cmd == NULL) {
            fprintf(stderr, "Error: rc shell not found\n");
            return -1;
        }
    } else {
        cmd = shell_cmd;
    }

    /* Create pipes for communication */
    if (pipe(stdin_pipe) < 0) {
        perror("pipe stdin");
        return -1;
    }

    if (pipe(stdout_pipe) < 0) {
        perror("pipe stdout");
        close(stdin_pipe[0]);
        close(stdin_pipe[1]);
        return -1;
    }

    /* Fork to create shell process */
    pid = fork();

    if (pid < 0) {
        perror("fork");
        close(stdin_pipe[0]);
        close(stdin_pipe[1]);
        close(stdout_pipe[0]);
        close(stdout_pipe[1]);
        return -1;
    }

    if (pid == 0) {
        /* Child process (shell) */

        /* Close unused pipe ends */
        close(stdin_pipe[1]);   /* Close write end of stdin */
        close(stdout_pipe[0]);  /* Close read end of stdout */

        /* Dup pipe ends to stdin/stdout */
        dup2(stdin_pipe[0], STDIN_FILENO);
        dup2(stdout_pipe[1], STDOUT_FILENO);
        dup2(stdout_pipe[1], STDERR_FILENO);

        /* Close original pipe ends after dup */
        close(stdin_pipe[0]);
        close(stdout_pipe[1]);

        /* Execute shell */
        execlp(cmd, cmd, NULL);

        /* If execlp returns, error occurred */
        fprintf(stderr, "Error: exec failed: %s\n", strerror(errno));
        exit(1);
    }

    /* Parent process */

    /* Close unused pipe ends */
    close(stdin_pipe[0]);   /* Close read end of stdin */
    close(stdout_pipe[1]);  /* Close write end of stdout */

    /* Store pipe FDs in window for I/O handling */
    /* TODO: We need to add these to KryonWindow structure */
    /* For now, we'll close them to prevent leaks */
    close(stdin_pipe[1]);
    close(stdout_pipe[0]);

    fprintf(stderr, "Spawned shell (pid=%d) in window %u\n", (int)pid, win->id);

    return 0;
}
