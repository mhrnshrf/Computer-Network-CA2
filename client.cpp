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











 void* read_console(void*){
    int i=0;

char line [500];

char usr[20];
char pass[20];
char token [20];
char port_no[10];
char service[100];
char file[100];
char access_type[10];
char data[1000];
char * command[20];
bzero(line,500);
bzero(usr,20);
bzero(pass,20);
bzero(token,20);
bzero(service,100);
bzero(access_type,10);
bzero(file,100);
bzero(data,1000);
bzero(port_no,10);

    read(0, line, sizeof(line));
    parse(line, command);
    strcpy(token, command[i++]);
        if (string(token) == "Login")
    {
        strcpy(usr, command[i++]);
        strcpy(pass, command[i++]);
        //to do:
        // fill the rout table and find shortest path



 }
 else if(string(token)=="Connect"){
    strcpy(token, command[i++]);
    if(string(token)=="Switch"){
        strcpy(port_no, command[i++]);
        // to do:
        // connect to special switch
        //set ip

    }
    else{
        cout<<"command not found!"<<endl;
        exit(1);
    }

 }
 else if(string(token)=="Get"){


 }
 else if(string(token)=="Request"){
    strcpy(service, command[i++]);
     strcpy(access_type, command[i++]);
 }

 else if(string(token)=="Send"){
    strcpy(file, command[i++]);
   
 }

 else if(string(token)=="Append"){
    strcpy(file, command[i++]);
    strcpy(data, command[i++]);
   
 }


 else if(string(token)=="Logout"){
   
 }
 else {
    cout<<"Command not found"<<endl;
 }

}










int main(int argc, char *argv[])
{

    //     pthread_t th;
    // int rc;
    // for (int i = 0; i < 100; ++i)
    // {
    //     rc = pthread_create(&th, NULL, &read_console, (void*)NULL);
    // }
 

    // int sfd, n, i, x;
    // socklen_t len;
    // char sline[MAX], rline[MAX+1];
    // struct sockaddr_in saddr;
    int i,x;
 
    // sfd = socket(AF_INET, SOCK_DGRAM, 0);
    // if (argc < 3) 
    // {
    //     char error[]="ERROR: NOT ENOUGH ARGUMNETS\n";
    //     write( 2, error, sizeof(error)-1);
    //     exit(1);
    // }

    // int ca_port_number = char_s_to_int(argv[2]);

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
    char data[20];
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
    bzero(data,20);
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

            len=sizeof(saddr);
            char dst[] = "0000";
            char type[] = "cl";
            char cmd[] = "ct"; 
            char* src;
            // TODO padding port_str
            itoa(x, src);
            encaps(type, cmd, dst, port_str, sline, packet);

            int ns = sendto(sfd, packet, strlen(packet), 0, (struct sockaddr *)&saddr, len);
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
