/*
 * Udp proxy
 *
 * server.c
 *
 *  Created on: Oct 14, 2015
 * Modified on: Sep 01, 2016
 *      Author: igor.delac@gmail.com
 */

#include "server.h"

/**
 * Terminate this application with custom message.
 * @param msg message to print
 */
void die(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * Start udp proxy service. Local port is udp port value on which proxy receive datagram.
 * All arguments are passed as string and are converted into integers.
 *
 * @param localport a string that contains valid udp port value
 * @param serverip a string that contains valid server ip address
 * @param serverport a string that contains valid server port
 */
void udpserver(char* localport, char* serverip, char* serverport)
{
    struct sockaddr_in si_me;

    int serverSocket;

    ssize_t recv_len;
    socklen_t slen = sizeof(si_me);

    char datagram[BUFLEN];

    /*
     * Create a server UDP socket.
     */
    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("Socket problem: UDP not supported.");
    }

    /*
     * Zero out the structure.
     */
    memset((char *) &si_me, 0, sizeof(si_me));

    /*
     * Bind to local udp port and all ip
     * interfaces.
     */
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(atoi(localport));
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    /*
     * Bind socket to port.
     */
    if( bind(serverSocket, (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("Bind error. Check that port is available.");
    }

    /*
     * UDP proxy socket address.
     */
    struct sockaddr_in si_proxy;
    int proxySocket;

    if ((proxySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        die("Socket problem: UDP not supported.");
    }

    memset((char *) &si_proxy, 0, sizeof(si_proxy));
    si_proxy.sin_family = AF_INET;
    si_proxy.sin_port = htons(atoi(serverport));

    if (inet_aton(serverip, &si_proxy.sin_addr) == 0) {
        die("Call inet_aton() failed for given server address.");
    }

    /*
     * Keep listening for data.
     */
    while (1)
    {
        struct sockaddr_in si_other;

        syslog(LOG_NOTICE, "Waiting for data ...");

        /*
         * Try to receive some data, this is a blocking call.
         */
        if ((recv_len = recvfrom(serverSocket, datagram, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            syslog(LOG_NOTICE, "Function recvfrom() problem.");
        }

        /*
         * Print details of the client data received.
         */
        syslog(LOG_NOTICE, "Received packet from: %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

        /*
         * Forward datagram to remote server.
         */
        syslog(LOG_NOTICE, "Forwarding packet to: %s:%d\n", inet_ntoa(si_proxy.sin_addr), ntohs(si_proxy.sin_port));
        if (sendto(proxySocket, datagram, recv_len, 0, (struct sockaddr*) &si_proxy, slen) == -1)
        {
            syslog(LOG_NOTICE, "Function sendto() problem.");
        }

        /*
         * Receive a reply from server on proxy socket.
         */
        if ((recv_len = recvfrom(proxySocket, datagram, BUFLEN, 0, (struct sockaddr *) &si_proxy, &slen)) == -1)
        {
            syslog(LOG_NOTICE, "Function recvfrom() problem.");
        }

        /*
         * Print details of the client data received.
         */
        syslog(LOG_NOTICE, "Received packet from: %s:%d\n", inet_ntoa(si_proxy.sin_addr), ntohs(si_proxy.sin_port));


        /*
         * Send back reply to client.
         */
        syslog(LOG_NOTICE, "Forwarding packet to: %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        if (sendto(serverSocket, datagram, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            syslog(LOG_NOTICE, "Function sendto() problem.");
        }
    }

    close(serverSocket);
    close(proxySocket);

}

