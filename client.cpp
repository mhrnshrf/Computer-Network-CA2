#include <stdlib.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "tknlib.h"
#include <fstream>	
#include <vector>	

using namespace std;


int main(int argc, char *argv[])
{
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
    char buffer[2000];
    char line[2000];
    char tmp[2000];
    char token[20];
    char usr[20];
    char pass[20];
    char service[20];
    char access[20];
    char data[20];
    char port_str[4];


    bzero(buffer,2000);
    bzero(command,20);
    bzero(line,2000);
    bzero(tmp,2000);
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
