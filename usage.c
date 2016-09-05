/*
 * Udp proxy
 *
 * usage.c
 *
 *  Created on: Oct 14, 2015
 * Modified on: Sep 01, 2016
 *      Author: igor.delac@gmail.com
 */

#include "usage.h"

/**
 * Print application usage and command line example.
 *
 * @param execname binary executable name, could be
 * provided as argv[0]
 */
void usage(char* execname) {

    printf("Usage:\n");
    printf(" -l [local udp port to listen]\n");
    printf(" -s [server ip address]\n");
    printf(" -p [remote server udp port]\n");
    printf("Example:\n");
    printf("%s -l 1234 -s 127.0.0.1 -p 1234\n", execname);
    printf("\n");

}
