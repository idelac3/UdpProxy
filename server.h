/*
 * Udp proxy
 *
 * server.h
 *
 *  Created on: Oct 14, 2015
 * Modified on: Sep 01, 2016
 *      Author: igor.delac@gmail.com
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>

#define BUFLEN 8192

void udpserver(char* localport, char* serverip, char* serverport);

#endif /* SERVER_H_ */

