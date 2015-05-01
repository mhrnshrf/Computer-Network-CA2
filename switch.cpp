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
#include "tknlib.h"

using namespace std;







 
int main(int argc, char *argv[])
{
    int i;
    struct timeval tv;
    tv.tv_sec = 1;  // 1 Secs Timeout 
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors

    map<int, int> table;

    for (int j = 2; j < argc; j++)
    {
        table[atoi(argv[j])] = atoi(argv[1]);
    }





    for(i = 1; i < argc; i++){

        int sfd, n;
        socklen_t len;
        char buf[128];
        struct sockaddr_in saddr, caddr;
     
        sfd = socket(AF_INET, SOCK_DGRAM, 0); 
        if(sfd == -1)
        {
            cerr << "Socket creation failed!" << endl;
            exit(1);
        }
        setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

     
        bzero(&saddr, sizeof(saddr));
        saddr.sin_family = AF_INET;
        saddr.sin_addr.s_addr = htonl(INADDR_ANY);
        saddr.sin_port = htons(atoi(argv[i]));
     
        int b = bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr));
        if(b == -1)
        {
            cerr << "Port bind failed!" << endl;
            exit(1);
        }
     
        cout << "Switch running...\n";
        cout << i << " PORT: " << argv[i] << endl;


        len=sizeof(caddr);
        bzero(buf,128);
        n=recvfrom(sfd, buf, 128, 0, (struct sockaddr *)&caddr, &len);
        // cout << buf << endl;
        int r = route(buf, argv, argc -1);
        // sendto(sfd, buf, n, 0, (struct sockaddr *)&caddr, len);
   
        
       
        // cout << "Timeout!\n";
        close(sfd);

        if (i == argc-1)
            i = 0;
        

    }
 
    return 0;
}