#ifndef REALTIMEPROJECT_CONF_H
#define REALTIMEPROJECT_CONF_H

#endif //REALTIMEPROJECT_CONF_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <signal.h>

//#define SERVER_PORT 80
static char buf[BUFSIZ];

struct get_struct{
    char* file_name;
};

struct msgbuf
{
    long type;
    char a[1024];
    int pid;
}p;
