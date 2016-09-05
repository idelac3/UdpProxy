/*
 * Udp proxy
 *
 * main.c
 *
 *  Created on: Oct 14, 2015
 * Modified on: Sep 01, 2016
 *      Author: igor.delac@gmail.com
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <getopt.h>

extern char *optarg;
extern int optind, opterr, optopt;

#include "usage.h"
#include "server.h"

void sighandler(int sig)
{
    if (sig == SIGTERM)
    {
        syslog(LOG_NOTICE, "Service terminated.");
        closelog();
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char **argv)
{
    int c;
    char* options = "l:s:p:hv";
    char* argval[] = {NULL, NULL, NULL};

    /*
     * Parse arguments and their values.
     */
    while ((c = getopt(argc, argv, options)) != -1)
    {
        switch (c)
        {
        case 'l':
            argval[0] = optarg;
            break;
        case 's':
            argval[1] = optarg;
            break;
        case 'p':
            argval[2] = optarg;
            break;
        case 'h':
        case 'v':
            usage(argv[0]);
            return EXIT_SUCCESS;
        case '?':
            printf("Unknown switch: %c. Ignoring.\n", optopt);
            break;
        default:
            continue;
        }
    }

    /*
     * Check that argument values are set.
     */
    if (argval[0] == NULL)
    {
        printf("Missing local udp port for listening.\n");
        return EXIT_FAILURE;
    }

    if (argval[1] == NULL)
    {
        printf("Missing server ip address.\n");
        return EXIT_FAILURE;
    }

    if (argval[2] == NULL)
    {
        printf("Missing server udp port number.\n");
        return EXIT_FAILURE;
    }

    /*
     * Start as service.
     */
    pid_t pid;

    pid = fork();

    /* Error in fork. */
    if (pid < 0)
    {
        return EXIT_FAILURE;
    }

    /* Terminate parent process. */
    if (pid > 0)
    {
        return EXIT_SUCCESS;
    }

    /* Child process becomes session leader. */
    if (setsid() < 0)
    {
        return EXIT_FAILURE;
    }

    /* Set new file permission */
    umask(0);

    /* Close file std. descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    /* Install sig. handler for kill command (default signal is TERM) */
    signal(SIGTERM, sighandler);

    /* Open syslog logger */
    openlog(argv[0], LOG_PID, LOG_DAEMON);
    syslog(LOG_NOTICE, "Service started.");

    /* Run main logic of udp proxy service. */
    udpserver(argval[0], argval[1], argval[2]);

    return EXIT_SUCCESS;
}

