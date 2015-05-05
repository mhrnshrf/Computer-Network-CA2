#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <strings.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <map>
#include <pthread.h>
#include "tknlib.h"

using namespace std;

int index(int port, char *argv[])
{
    for (int i = 1; i < 10; ++i)
    {
        if (atoi(argv[i]) == port)
            return i-1;
        
    }
}


int main(int argc, char *argv[])
{
    struct timeval tv;
    tv.tv_sec = 1;  // 1 Secs Timeout 
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors
    int sfd[10];
    socklen_t len;
    char buf[128];
    char dum[128];
    struct sockaddr_in saddr[10], caddr[10];
    struct sockaddr_in naddr;

    // create socket
    for (int i = 0; i < argc-1; ++i)
    {
        sfd[i] = socket(AF_INET, SOCK_DGRAM, 0); 
        if(sfd[i] == -1)
        {
            cerr << "Socket creation failed!" << endl;
            exit(1);
        }
        bzero(&saddr[i], sizeof(saddr[i]));
        saddr[i].sin_family = AF_INET;
        saddr[i].sin_addr.s_addr = htonl(INADDR_ANY);
        saddr[i].sin_port = htons(atoi(argv[i+1]));
        cout << i << " sin_port: " << saddr[i].sin_port << endl;
     
        int b = bind(sfd[i], (struct sockaddr *)&saddr[i], sizeof(saddr[i]));
        if(b == -1)
        {
            cerr << "Port bind failed!" << endl;
            exit(1);
        }

        setsockopt(sfd[i], SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
    }
 
    // receive, route & send packet
    for(int i = 0; i < argc-1; i++){

    len=sizeof(caddr[i]);
    bzero(buf,128);

    int nr = recvfrom(sfd[i], buf, 128, 0, (struct sockaddr *)&caddr[i], &len);

        // cout << i <<" cadder: " << caddr[i].sin_port << endl;
    

    if (nr != -1)
    {
        int r = route(buf, argv, argc -1);
        int dst = index(r, argv);


        int ns = sendto(sfd[i], buf, nr, 0, (struct sockaddr *)&caddr[i], len);
        if (ns == -1)
        {
            cerr << "Send failed!" << endl;
            exit(1);
        }
    }
    
   
    // cout << "Timeout!\n";
    // close(sfd);

    if (i == argc-2)
        i = -1;
    

    }
 
    return 0;
}