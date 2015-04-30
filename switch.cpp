#include <stdio.h>
#include <sys/socket.h>
#include <strings.h>
#include <iostream>
#include <netinet/in.h>
#include "tknlib.h"

using namespace std;
 
int main()
{
    int sfd, n;
    socklen_t len;
    char line[128];
    struct sockaddr_in saddr, caddr;
 
    sfd = socket(AF_INET, SOCK_DGRAM, 0);   
 
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(2910);
 
    bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr));
 
    printf("Switch running...\n");
    // for(;;) {
        len=sizeof(caddr);
        n=recvfrom(sfd, line, 128, 0, (struct sockaddr *)&caddr, &len);
        cout << line << endl;
        sendto(sfd, line, n, 0, (struct sockaddr *)&caddr, len);
    // }
 
    return 0;
}