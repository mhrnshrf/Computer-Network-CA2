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
vector<int> cl;
vector<int> ip;
int server;


void *read_console(void* port)
{

    int i = 0;
    int* portpoint = (int *) port;
    int myport = *portpoint;
    char *command[20];
    char line[128];
    char tmp[128];
    char token[20];
    char port_str[8];
    char mystr[8];
    bzero(port_str,8);
    bzero(mystr,8);
    itoa(myport, mystr);

    int sw_portno;
   

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
            encaps("sw", "ct", "00000000", mystr, "imswitch", buf);            
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
    char dum[128];
    char swtype[] = "sw";
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
            cout << buf << endl;
            // cout << "is 1st? " << isfirst(buf) << endl;
            
            if (issw(buf))
            {
                if (isfirst(buf))
                {
                    // TODO     
                    sw.push_back(getsrc(buf));
                    
                    cout << "sw connected to me: " << getsrc(buf) << endl;
                }
            }
            else if (iscl(buf))
            {
                // TODO 
                if (isfirst(buf))
                {
                    // cout << "sdjfgggggggggjjfddddddddddddddddddddd";
                    // TODO dec ttl
                    // for (int i = 0; i < sw.size(); ++i)
                    // {
                    //     caddr.sin_port = sw[i];
                    //     int ns = sendto(sfd, buf, strlen(buf), 0, (struct sockaddr *)&caddr, len);
                    //     if (ns == -1)
                    //     {
                    //         cerr << "Send failed!" << endl;
                    //         exit(1);
                    //     } 
                    // }

                    cl.push_back(caddr.sin_port);
                    ip.push_back(getsrc(buf));

                    caddr.sin_family = AF_INET;
                    inet_pton(AF_INET, "localhost", &caddr.sin_addr);
                    caddr.sin_port = htons(server);

                    chtype(buf, swtype);


                    cout << "send to server: " << buf << endl;

                    int ns = sendto(sfd, buf, nr, 0, (struct sockaddr *)&caddr, len);
                    if (ns == -1)
                    {
                        cerr << "Send failed!" << endl;
                        exit(1);
                    }
                }
            }            
            else if (issr(buf))
            {
                // TODO 
                if (isfirst(buf))
                {
                    server = getsrc(buf);
                    cout << "server connected on: " << server << endl;

                }
                else
                {
                    int target = getdst(buf);
                    for (int i = 0; i < ip.size(); ++i)
                    {
                        if (ip[i] == target)
                        {
                            caddr.sin_port = cl[i];
                            int ns = sendto(sfd, buf, nr, 0, (struct sockaddr *)&caddr, len);
                            if (ns == -1)
                            {
                                cerr << "Send failed!" << endl;
                                exit(1);
                            }  
                        }
                    }
                }



            }

        }
        
       
        // cout << "Timeout!\n";
        // close(sfd);


    }
 
    return 0;
}