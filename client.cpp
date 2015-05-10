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
    char usr[3];
    bzero(usr,3); 
    bool login = false;
    char ipstr[8];

    int sw_portno;
    int sfd, n;
    socklen_t len;
    struct sockaddr_in saddr;

while(1)
{

    char *command[20];
    char buf[128];
    char packet[128];
    char tmp[128];
    char line[128];
    char token[20];
    char pass[20];
    char data[101];
    char port_str[4];

    char service[6];
    char access[7];
    bzero(service, 6);
    bzero(access, 7);

    bzero(buf,128);
    bzero(packet,128);
    bzero(command,20);
    bzero(line,128);
    bzero(tmp,128);
    bzero(token,20);
    bzero(pass,20);
    bzero(data,100);
    bzero(port_str,4);
    
    i = 0;
    // char sline[MAX], rline[MAX+1];


    
    read(0, line, sizeof(line));
    strcpy(tmp, line);
    int p = parse(line, command);

    strcpy(token, command[i++]);

    if (string(token) == "Login")
    {
        // TODO 
        strcpy(usr , command[i++]);
        strcpy(pass , command[i++]);
        login = true;

    }
    else if (string(token) == "Connect")
    {
        strcpy(token, command[i++]);

        if(string(token) == "Switch")
        {
            strcpy(port_str, command[i++]);
            sw_portno = atoi(port_str);

            // TODO
            if (login)
            {
                x = setip();
                bzero(ipstr,8);
                itoa(x, ipstr);

                padding(ipstr, 8);

                cout << x << endl;


                sfd = socket(AF_INET, SOCK_DGRAM, 0);   
             
                bzero(&saddr, sizeof(saddr));
                saddr.sin_family = AF_INET;
                inet_pton(AF_INET, argv[1], &saddr.sin_addr);
                saddr.sin_port = htons(sw_portno);  
             
                printf("Client running...\n");

                bzero(buf, 128);
                encaps("cl", "ct", "00001000", ipstr, "imclient", buf);

                char sr[] = "sr";
                char dest[] = "5885";
                char src[] = "2323";
                padding(src, 8);
                padding(dest, 8);
                // cout << "user: " << usr << endl;
                // cout << "isfirst? " << isfirst(buf) << endl; 
                // cout << "reqlist? " << reqlist(buf) << endl; 
                // cout << "iscl? " << iscl(buf) << endl; 
                // itoa(saddr.sin_port, dest);
                // cout << "src " << strlen(src) << endl;

                // cout << "ip: " << src << endl;
                // cout << "Buffer is : " << buf << endl;
                // chtype(buf, sr);
                // cout << "Buffer is : " << buf << endl;
                // chdst(buf, src);
                // cout << "Buffer is : " << buf << endl;
                // chsrc(buf, dest);
                // cout << "Buffer is : " << buf << endl;
                // chdata(buf, usr);
                // cout << "Buffer is : " << buf << endl;

                // // cout << "1st?  " << isfirst(buf) << endl;
                // chcmd(buf, sr);
                // cout << "src: " << getdst(buf) << endl;
                // if (issr(buf))
                //     cout << "sr\n";
                // if (isfirst(buf))
                //     cout << "1st\n";

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
            }
            else
            {
                cout << "First Login please!" << endl;
            }



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
                    encaps("cl", "gl", "00001000", ipstr, "givelistplz", buf);
                    chdata(buf, usr);
        
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

                    bzero(data, 101);
                    copy(buf+27, buf+128, data);
                    cout << data << endl;
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
        if (string(access) == "read" )
            encaps("cl", "qr", "00001000", ipstr, service, buf);        
        else if (string(access) == "write" )
            encaps("cl", "qw", "00001000", ipstr, service, buf);
        else if (string(access) == "append" )
            encaps("cl", "qa", "00001000", ipstr, service, buf);

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

        bzero(data, 101);
        copy(buf+27, buf+128, data);
        cout << data << endl;



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
        close(sfd);
        login = false;
        // exit(1);
    }
    else
    {
        cout << "Command not found!" << endl;
        exit(1);
    }


}

    
    
    return 0;
}
