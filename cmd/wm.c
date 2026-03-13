/*
 * Kryon WM - Standalone Window Manager Binary
 * C89/C90 compliant
 *
 * Simple wrapper that:
 * 1. Connects to marrow (or checks if already connected)
 * 2. Starts kryon WM with default configuration
 * 3. Runs until WM exits
 *
 * Usage: wm [--marrow ADDR] [--run FILE.kry]
 */

#include "p9client.h"
#include "marrow.h"
#include <lib9.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

/*
 * Signal handler for graceful shutdown
 */
static volatile int running = 1;

static void signal_handler(int sig)
{
    (void)sig;
    running = 0;
}

/*
 * Print usage
 */
static void print_usage(const char *progname)
{
    fprintf(stderr, "Usage: %s [OPTIONS]\n", progname);
    fprintf(stderr, "\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  --marrow ADDR        Marrow server address (default: tcp!localhost!17010)\n");
    fprintf(stderr, "  --run FILE.kry       Load and run the specified .kry file\n");
    fprintf(stderr, "  --help               Show this help message\n");
    fprintf(stderr, "\n");
}

/*
 * Main entry point
 */
int main(int argc, char **argv)
{
    char *marrow_addr;
    char *load_file;
    int i;
    P9Client *client;

    /* Default values */
    marrow_addr = "tcp!localhost!17010";
    load_file = NULL;

    /* Parse arguments */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--marrow") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --marrow requires an argument\n");
                return 1;
            }
            marrow_addr = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "--run") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --run requires a file argument\n");
                return 1;
            }
            load_file = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else {
            fprintf(stderr, "Error: unknown option '%s'\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    /* Check if default .kry file exists */
    if (load_file == NULL) {
        /* Try default.kry in current directory */
        if (access("default.kry", R_OK) == 0) {
            load_file = "default.kry";
        }
    }

    /* Setup signal handlers */
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    /* Connect to Marrow */
    fprintf(stderr, "Connecting to Marrow at %s...\n", marrow_addr);

    client = p9_connect(marrow_addr);
    if (client == NULL) {
        fprintf(stderr, "Error: failed to connect to Marrow at %s\n", marrow_addr);
        fprintf(stderr, "Is Marrow running? Start it with: ./amd64/bin/marrow\n");
        return 1;
    }

    /* Authenticate with Marrow */
    if (p9_authenticate(client, 0, "none", "") < 0) {
        fprintf(stderr, "Error: failed to authenticate with Marrow\n");
        p9_disconnect(client);
        return 1;
    }

    fprintf(stderr, "Connected to Marrow\n");

    /*
     * TODO: Implement actual WM startup
     * For now, this is a placeholder that demonstrates the connection
     *
     * The full implementation would:
     * 1. Initialize the WM subsystems
     * 2. Load the .kry file
     * 3. Run the event loop
     */

    if (load_file != NULL) {
        fprintf(stderr, "Loading WM configuration from: %s\n", load_file);
        fprintf(stderr, "Note: Full WM startup not yet implemented\n");
    } else {
        fprintf(stderr, "No WM configuration file specified\n");
        fprintf(stderr, "Use --run FILE.kry to specify a configuration\n");
    }

    /* Wait for signal */
    fprintf(stderr, "WM waiting for Ctrl-C...\n");
    while (running) {
        sleep(1);
    }

    /* Cleanup */
    fprintf(stderr, "\nShutting down...\n");
    p9_disconnect(client);

    fprintf(stderr, "Shutdown complete\n");

    return 0;
}
