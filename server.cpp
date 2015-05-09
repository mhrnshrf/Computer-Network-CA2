#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <strings.h>
#include <iostream>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "tknlib.h"
#include <map>
#include <vector>





using namespace std;

vector<int> sw;



void *read_console(void* port)
{

    int i = 0;
    int* portpoint = (int *) port;
    int myport = *portpoint;
    int sw_portno;
    char *command[20];
    char line[128];
    char tmp[128];
    char token[20];
    char port_str[8];
    char mystr[8];
    bzero(port_str,8);
    bzero(mystr,8);
    itoa(myport, mystr);

    int sfd;
    socklen_t len;
    char buf[128];
    struct sockaddr_in saddr;



    read(0, line, sizeof(line));
    strcpy(tmp, line);
    int p = parse(line, command);
    strcpy(token, command[i++]);

    if (string(token) == "Connect")
    {
        strcpy(token, command[i++]);

        if(string(token) == "Switch")
        {
            strcpy(port_str, command[i++]);
            sw_portno = atoi(port_str);

            // TODO

            cout <<  "I connect to: " << sw_portno << endl;

            sw.push_back(sw_portno);

            // create socket
            bzero(&saddr, sizeof(saddr));
            saddr.sin_family = AF_INET;
            saddr.sin_addr.s_addr = htonl(INADDR_ANY);
            saddr.sin_port = htons(sw_portno);

            sfd = socket(AF_INET, SOCK_DGRAM, 0); 
            if(sfd == -1)
            {
                cerr << "Socket creation failed!" << endl;
                exit(1);
            }

            len = sizeof(saddr);
            bzero(buf, 128);
            padding(mystr, 8);
            encaps("sr", "ct", "00000000", mystr, "imserver", buf);
            int ns = sendto(sfd, buf, strlen(buf), 0, (struct sockaddr *)&saddr, len);
            if (ns == -1)
            {
                cerr << "Send failed!" << endl;
                exit(1);
            }
            sw.push_back(sw_portno);

        }
        else
        {
            cout << "Command not found!" << endl;
            exit(1);
        }


    }
    else
    {
        cout << "Command not found!" << endl;
        exit(1);
    }

    pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
    int sfd;
    int port;
    port = atoi(argv[1]);
    socklen_t len;
    char buf[128];
    char srtype[] = "sr";
    struct sockaddr_in saddr, caddr;

    pthread_t th;
    int rc;
    for (int i = 0; i < 10; ++i)
    {
        rc = pthread_create(&th, NULL, &read_console, (void*)&port);
    }


    // create socket
    sfd = socket(AF_INET, SOCK_DGRAM, 0); 
    if(sfd == -1)
    {
        cerr << "Socket creation failed!" << endl;
        exit(1);
    }

    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(atoi(argv[1]));
 
    int b = bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(b == -1)
    {
        cerr << "Port bind failed!" << endl;
        exit(1);
    }

    // receive, route & send packet
    while(1)
    {

        len=sizeof(caddr);
        bzero(buf,128);

        int nr = recvfrom(sfd, buf, 128, 0, (struct sockaddr *)&caddr, &len);

            // cout << i <<" cadder: " << caddr[i].sin_port << endl;


        if (nr != -1)
        {
            cout << "Received packet from " << inet_ntoa(caddr.sin_addr)<<  ":" << ntohs(caddr.sin_port) << endl;
            
            if (issw(buf))
            {
                char dst[8];
                char src[8];
                char rep[2];
                bzero(rep, 2);
                bzero(dst, 8);
                bzero(src, 8);
                strcpy(rep, "rp");
                strcpy(src, "00001000");
                itoa(getsrc(buf), dst);
                padding(dst, 8);
                strcat(buf,"SERVER");
                chsrc(buf, src);
                chdst(buf, dst);
                chcmd(buf, rep);
                chtype(buf, srtype);
                int ns = sendto(sfd, buf, strlen(buf), 0, (struct sockaddr *)&caddr, len);
                if (ns == -1)
                {
                    cerr << "Send failed!" << endl;
                    exit(1);
                }
                
            }
            else if (issp(buf))
            {
                // TODO 


            }            


        }
        
       
        // close(sfd);


    }
 
    return 0;
}