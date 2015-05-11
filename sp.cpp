#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include "tknlib.h"

using namespace std;



int main(int argc, char *argv[])
{

    int i,x;
    char serv[5];
    char sp[] = "sp";
    bzero(serv, 5);
    strcpy(serv, argv[1]);



    char *command[20];
    char buf[128];
    char packet[128];
    char line[128];
    char tmp[128];
    char token[20];
    char usr[3];
    char pass[20];
    char service[20];
    char access[20];
    char data[100];
    char port_str[4];


    bzero(buf,128);
    bzero(packet,128);
    bzero(line,128);
    bzero(command,20);
    bzero(tmp,128);
    bzero(token,20);
    bzero(usr,3);    
    bzero(pass,20);
    bzero(service,20);
    bzero(access,20);
    bzero(data,100);
    bzero(port_str,4);
    
    i = 0;
    int sr_portno;
    char ipstr[] = "10102020";
    int sfd, n;
    socklen_t len;
    struct sockaddr_in saddr;


    
    read(0, line, sizeof(line));
    strcpy(tmp, line);
    int p = parse(line, command);

    strcpy(token, command[i++]);

    if (string(token) == "Connect")
    {
        strcpy(token, command[i++]);

        if(string(token) == "Server")
        {
            strcpy(port_str, command[i++]);
            sr_portno = atoi(port_str);

            // TODO

            sfd = socket(AF_INET, SOCK_DGRAM, 0);   
         
            bzero(&saddr, sizeof(saddr));
            saddr.sin_family = AF_INET;
            inet_pton(AF_INET, "localhost", &saddr.sin_addr);
            saddr.sin_port = htons(sr_portno);  
         
            printf("Service Provider running...\n");

            bzero(buf, 128);
            encaps("sp", "ct", "00001000", ipstr, serv, buf);

            cout << "Im sending: " << buf << endl;

            len=sizeof(saddr);
            int ns = sendto(sfd, buf, strlen(buf), 0, (struct sockaddr *)&saddr, len);
            if (ns == -1)
            {
                cerr << "Send failed!" << endl;
                exit(1);
            } 

            bzero(buf,128);
            
            int nr = recvfrom(sfd, buf, 128, 0, NULL, NULL);
            if (nr == -1)
            {
                cerr << "Receive failed!" << endl;
                exit(1);
            } 

            cout << buf << endl;

            while(1)
            {

                bzero(buf,128);
                
                int nr = recvfrom(sfd, buf, 128, 0, NULL, NULL);
                if (nr == -1)
                {
                    cerr << "Receive failed!" << endl;
                    exit(1);
                }

                if (reqacc(buf))
                {
                    ifstream in;
                    char addr[50];
                    bzero(addr, 50);
                    strcpy(addr, "/home/me/Desktop/");
                    strcat(addr, argv[1]);
                    in.open(addr);
                    char data[101];
                    bzero(data, 101);
                    string ln;

                    while(!in.eof())
                    {
                        getline(in, ln);
                        strcat(data, ln.c_str());
                        strcat(data, "\n");
                    }

                    chtype(buf, sp);
                    chdata(buf, data);

                    len=sizeof(saddr);
                    int ns = sendto(sfd, buf, strlen(buf), 0, (struct sockaddr *)&saddr, len);
                    if (ns == -1)
                    {
                        cerr << "Send failed!" << endl;
                        exit(1);
                    } 
                } 

            }

            



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




    
    
    return 0;
}
