# Udp Proxy

This is example code how to do an udp proxy, code is in C written.

Main goal is receive an udp datagram from client and forward it to server. Then read datagram which is sent from server and forward it back to client.

## Compile source

Use make utility to build source code:

<PRE>
igor@eclipseVM:~/workspace/UdpProxy$ make
gcc -c main.c
gcc -c server.c
gcc -c usage.c
gcc -o UdpProxy *.o
igor@eclipseVM:~/workspace/UdpProxy$
</PRE>

Result is an executable file in same folder as source.

Just gcc and make are needed, no other libraries are required.

## Basic usage

To see all available options, use -h argument for help:

<PRE>
igor@eclipseVM:~/workspace/UdpProxy$ ./UdpProxy -h
Usage:
 -l [local udp port to listen]
 -s [server ip address]
 -p [remote server udp port]
Example:
./UdpProxy -l 1234 -s 127.0.0.1 -p 1234

igor@eclipseVM:~/workspace/UdpProxy$
</PRE>

Local port is port on which proxy will receive datagram, and forward it to server.
Server ip address and port is destination where to forward datagram from client.

Once the datagram is forwarded to server, proxy will wait for server to send back something as an answer (some datagram). Answer will be forwarded to client. This is sufficient for simple udp protocols like SNMP.

Udp proxy will write log information to syslog service.

<PRE>
Sep  5 14:13:47 eclipseVM ./UdpProxy[2991]: Service started.
Sep  5 14:14:04 eclipseVM ./UdpProxy[2765]: Received packet from: 127.0.0.1:52585
Sep  5 14:14:04 eclipseVM ./UdpProxy[2765]: Forwarding packet to: 127.0.0.1:5555
Sep  5 14:14:12 eclipseVM ./UdpProxy[2765]: Received packet from: 127.0.0.1:5555
Sep  5 14:14:12 eclipseVM ./UdpProxy[2765]: Forwarding packet to: 127.0.0.1:52585
Sep  5 14:14:12 eclipseVM ./UdpProxy[2765]: Waiting for data ...
</PRE>

This will help to diagnose potential problems when forwarding udp datagrams.

To stop service use kill command:

<PRE>
igor@eclipseVM:~/workspace/UdpProxy$ kill 2765
</PRE>

and check syslog form appropriate message:

<PRE>
igor@eclipseVM:~/workspace/UdpProxy$ grep Udp /var/log/syslog | tail -n1
Sep  5 14:16:33 eclipseVM ./UdpProxy[2765]: Service terminated.
igor@eclipseVM:~/workspace/UdpProxy$ 
</PRE>

Code has been compiled and tested under Ubuntu and SuSE Linux distributions.

Author:
igor.delac@gmail.com