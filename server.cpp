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
#include <fstream>
#include <vector>



using namespace std;

vector<int> sw;
vector<int> sp;
vector<int> ip;
vector<int> corres;
vector<string> name;
vector<string> service;
vector<int> tbl[20];

string findname(char* buf)
{
    for (int i = 0; i < ip.size(); ++i)
    {
        if (ip[i] == getsrc(buf))
        {
            return name[i];
        }
    }
}

int findsp(string serv)
{
    for (int i = 0; i < service.size(); ++i)
    {
        if (serv == service[i])
        {
            return sp[i];
        }
    }
}

int findsw(int cl)
{
    for (int i = 0; i < ip.size(); ++i)
    {
        if (cl == ip[i])
        {
            return corres[i];
        }
    }
}

bool exist(int cl)
{
    for (int i = 0; i < ip.size(); ++i)
    {
        if (ip[i] == cl)
        {
            return true;
        }
    }
    return false;
}

bool allowed(string cl, char* serv, int cmd)
{
    ifstream in;
    string usr, file, perm;
    in.open("/home/me/Desktop/AC.txt");

    while(!in.eof())
    {
        in >> usr >> file >> perm ;
        if (usr == cl)
        {
            if (file == string(serv))
            {
                if (perm == "Read" &&  cmd == 1)
                    return true;
                else if (perm == "Append" &&  cmd == 2)
                    return true;
                else if (perm == "Write" &&  cmd == 3)
                    return true;
            }
        }
    }
    return false;
}



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
    char message[128];
    char data[100];

    char srtype[] = "sr";
    struct sockaddr_in saddr, caddr;

    // char cl[] = "A";
    // char serv[5] = "1.txt";
    // char cmd[] = "Read";

    // allowed(cl, serv, cmd);

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



        if (nr != -1)
        {
            cout << "Received packet from " << inet_ntoa(caddr.sin_addr)<<  ":" << ntohs(caddr.sin_port) << endl;
            // cout << buf << endl;
            int swport = caddr.sin_port;
            
            if (issw(buf))
            {
                // if (!exist(getsrc(buf)))
                // {
                // }
                if (reqlist(buf))
                {
                    ip.push_back(getsrc(buf));
                    corres.push_back(ntohs(caddr.sin_port));
                    char usr[4];
                    bzero(usr, 4);
                    copy(buf+27, buf+30, usr);
                    usr[3] = '\0';
                    name.push_back(string(usr));

                    char tmp[6];
                    bzero(data, 100);
                    bzero(tmp, 6);
                    strcpy(data, service[0].c_str());
                    for (int i = 1; i < service.size(); ++i)
                    {
                        strcat(data, "\n");
                        strcpy(tmp, service[i].c_str());
                        strcat(data, tmp);
                    }
                    chdata(buf, data);
                }
                else if (reqacc(buf))
                {
                    string cl = findname(buf);
                    char serv[6];
                    bzero(serv, 6);
                    copy(buf+27, buf+32, serv);
                    serv[5] = '\0';
                    int cmd = 0;
                    bzero(data, 100);

                    if (reqrd(buf))
                    {
                        cmd = 1;
                        strcpy(data, ":Reading ");

                    }
                    else if (reqap(buf))
                    {
                        cmd = 2;
                        strcpy(data, ":Appending ");
                    }
                    else if (reqwr(buf))
                    {
                        cmd = 3;
                        strcpy(data, ":Writing ");
                    }
                    strcat(data, serv);
                    strcat(data, " ");
                    if (allowed(cl, serv, cmd))
                    {
                        strcat(data, "is ");
                        buf[26] = '1';
                        caddr.sin_port = findsp(string(serv));
                        int ns = sendto(sfd, buf, strlen(buf), 0, (struct sockaddr *)&caddr, len);
                        if (ns == -1)
                        {
                            cerr << "Send failed!" << endl;
                            exit(1);
                        }

                    }
                    else
                    {
                        strcat(data, "isn't ");
                        buf[26] = '0';
                    }
                    strcat(data, "available for you!");
                    chdata(buf, data);
                }
                else if (isap(buf))
                {
                    string cl = findname(buf);
                    char serv[6];
                    bzero(serv, 6);
                    copy(buf+27, buf+32, serv);
                    serv[5] = '\0';
                    bzero(data, 100);

                    if (allowed(cl, serv, 2))
                    {
                        strcpy(data, ":Your data will be appended!");

                        caddr.sin_port = findsp(string(serv));
                        int ns = sendto(sfd, buf, strlen(buf), 0, (struct sockaddr *)&caddr, len);
                        if (ns == -1)
                        {
                            cerr << "Send failed!" << endl;
                            exit(1);
                        }
                    }
                    else
                    {
                        strcpy(data, ":Appending this file isn't available for you!");
                    }
                    chdata(buf, data);
                    
                }

                
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

                chsrc(buf, src);
                chdst(buf, dst);
                chcmd(buf, rep);
                chtype(buf, srtype);

                caddr.sin_port = swport;

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
                if (isfirst(buf))
                {
                    char serv[6];
                    bzero(serv, 6);
                    copy(buf+27, buf+32, serv);
                    serv[5] = '\0';
                    sp.push_back(caddr.sin_port);
                    service.push_back(string(serv));

                    bzero(message, 128);
                    strcpy(message, ":Your service is added!");
                    int ns = sendto(sfd, message, strlen(message), 0, (struct sockaddr *)&caddr, len);
                    if (ns == -1)
                    {
                        cerr << "Send failed!" << endl;
                        exit(1);
                    }
                }
                else
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

                    chsrc(buf, src);
                    chdst(buf, dst);
                    chcmd(buf, rep);
                    chtype(buf, srtype);

                    caddr.sin_family = AF_INET;
                    inet_pton(AF_INET, "localhost", &caddr.sin_addr);
                    caddr.sin_port = htons(findsw(getdst(buf)));

                    int ns = sendto(sfd, buf, strlen(buf), 0, (struct sockaddr *)&caddr, len);
                    if (ns == -1)
                    {
                        cerr << "Send failed!" << endl;
                        exit(1);
                    }


                }
            }            


        }
        
       
        // close(sfd);


    }
 
    return 0;
}