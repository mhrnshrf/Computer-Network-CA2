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
#define MAX 95 
#define DEST "00000000" 

using namespace std;

int setip()
{
    int ip;
    char str[4];
    ifstream in;
    ofstream out;
    in.open("/home/me/Desktop/ip");
    in >> str;
    in.close();
    ip = atoi(str);
    itoa(ip+1, str);    
    out.open("/home/me/Desktop/ip");
    out << str;
    out.close();

    return ip;
}



int main(int argc, char *argv[])
{

    int i,x;

while(1)
{

    char *command[20];
    char buf[128];
    char packet[128];
    char tmp[128];
    char line[128];
    char token[20];
    char usr[20];
    char pass[20];
    char service[20];
    char access[20];
    char data[100];
    char port_str[4];


    bzero(buf,128);
    bzero(packet,128);
    bzero(command,20);
    bzero(line,128);
    bzero(tmp,128);
    bzero(token,20);
    bzero(usr,20);    
    bzero(pass,20);
    bzero(service,20);
    bzero(access,20);
    bzero(data,100);
    bzero(port_str,4);
    
    i = 0;
    int sw_portno;


    
    read(0, line, sizeof(line));
    strcpy(tmp, line);
    int p = parse(line, command);

    strcpy(token, command[i++]);

    if (string(token) == "Login")
    {
        strcpy(usr , command[i++]);
        strcpy(pass , command[i++]);
        // TODO 

    }
    else if (string(token) == "Connect")
    {
        strcpy(token, command[i++]);

        if(string(token) == "Switch")
        {
            strcpy(port_str, command[i++]);
            sw_portno = atoi(port_str);

            // TODO
            cout << sw_portno << endl;
            x = setip();
            char ipstr[8];
            bzero(ipstr,8);
            itoa(x, ipstr);

            padding(ipstr, 8);

            cout << x << endl;

            int sfd, n;
            socklen_t len;
            char sline[MAX], rline[MAX+1];
            struct sockaddr_in saddr;

            sfd = socket(AF_INET, SOCK_DGRAM, 0);   
         
            bzero(&saddr, sizeof(saddr));
            saddr.sin_family = AF_INET;
            inet_pton(AF_INET, argv[1], &saddr.sin_addr);
            saddr.sin_port = htons(sw_portno);  
         
            printf("Client running...\n");

            bzero(buf, 128);
            encaps("cl", "ct", "00009999", ipstr, "salam", buf);
            char sr[] = "sr";
            char dest[] = "5885";
            char src[] = "2323";
            padding(src, 8);
            padding(dest, 8);

            // itoa(saddr.sin_port, dest);
            // cout << "src " << strlen(src) << endl;

            // cout << "ip: " << src << endl;
            // cout << "Buffer is : " << buf << endl;
            // cout << "Buffer is : " << buf << endl;
            // chtype(buf, sr);
            // chdst(buf, src);
            // chsrc(buf, dest);

            // // cout << "1st?  " << isfirst(buf) << endl;
            cout << "Im sending: " << buf << endl;
            // cout << "src: " << getdst(buf) << endl;


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



        }
        else
        {
            cout << "Command not found!" << endl;
            exit(1);
        }
        
    }  
    else if (string(token) == "Get")
    {
        strcpy(token, command[i++]);

        if(string(token) == "List")
        {
            strcpy(token, command[i++]);

            if(string(token) == "of")
            {
                strcpy(token, command[i++]);

                if(string(token) == "Services")
                {
                    // TODO
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
        }
        else
        {
            cout << "Command not found!" << endl;
            exit(1);
        }
        
    }  
    else if (string(token) == "Request")
    {
        strcpy(service, command[i++]);
        strcpy(access, command[i++]);

        // TODO

    }    
    else if (string(token) == "Send")
    {
        strcpy(service, command[i++]);
        // TODO
    }
    else if (string(token) == "Append")
    {
        strcpy(service, command[i++]);
        strcpy(data, command[i++]);
        // TODO
    }
    else if (string(token) == "Logout")
    {
        // TODO
    }
    else
    {
        cout << "Command not found!" << endl;
        exit(1);
    }


}

    
    
    return 0;
}
