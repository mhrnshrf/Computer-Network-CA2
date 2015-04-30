#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/time.h>
#include <errno.h>
#include "tknlib.h"
#include <fstream>			
#include <pthread.h>
#include <vector>

using namespace std;

struct tm start;
struct tm finish;

void set_voting_time(char* s, char* f)
{

memset(&start, 0, sizeof(struct tm));
memset(&finish, 0, sizeof(struct tm));

    char* s_h= strtok(s,":");
    char* s_min= strtok(NULL,":");
    char* f_h= strtok(f,":");
    char* f_min= strtok(NULL,":");

    //get current time
  
///////////////
start.tm_hour=atoi(s_h);
start.tm_min=atoi(s_min);
/////////////

finish.tm_hour=atoi(f_h);
finish.tm_min=atoi(f_min);


}
void extend(char* f)
{


    char* f_h= strtok(f,":");
    char* f_min= strtok(NULL,":");

    //get current time
  

finish.tm_hour=atoi(f_h);
finish.tm_min=atoi(f_min);


}
bool validate_time(){
    
time_t current;
time(&current);
cout<<ctime(&current)<<endl;
// cout<<start.tm_hour<<":"<<start.tm_min<<endl;

struct tm *cur= localtime(&current);
finish.tm_sec=cur->tm_sec;
finish.tm_year=cur->tm_year;
finish.tm_mon=cur->tm_mon;
finish.tm_mday=cur->tm_mday;
finish.tm_wday=cur->tm_wday;
finish.tm_yday=cur->tm_yday;
start.tm_sec=cur->tm_sec;
start.tm_year=cur->tm_year;
start.tm_mon=cur->tm_mon;
start.tm_mday=cur->tm_mday;
start.tm_wday=cur->tm_wday;
start.tm_yday=cur->tm_yday;
// cout<<start.tm_hour<<":"<<start.tm_min<<endl;
// cout<<finish.tm_hour<<":"<<finish.tm_min<<endl;
if(difftime(current,mktime(&start))<0)
  return false;
if(difftime(mktime(&finish),current)<0)
return false;
//  return false;

return true;
}

int char_s_to_int(char *char_s)
{
    int number = 0;
    while((*char_s >= '0') && (*char_s <= '9'))
    {
        number = (number * 10) + (*char_s - '0');
        char_s++;
    }
    return number;
}

struct person
{
    char code[3];
    char name[20];
    int vote;   
};

struct votebox
{
    vector<person> candids;
    int boxNum;
};

vector<person> candids;
vector<votebox> allbox;

void update_result(vector<person> candids, vector<votebox> allbox)
{

    ofstream out;
    char addr[50] = "/home/me/Desktop/net/Votes/";
    char filename[5];

    strcat(addr, "overall");
    out.open(addr);
    if (!out.is_open())
        cout << "Error opening file!" << endl;
    
    for (int i = 0; i < candids.size(); ++i)
    {
        out << candids[i].code << " " << candids[i].name << " " << candids[i].vote << endl;
    }
    
    out.close();



    for (int k = 1; k < allbox.size(); k++)
    {
        bzero(addr, 50);
        bzero(filename, 5);
        itoa(allbox[k].boxNum, filename);
        strcpy(addr,"/home/me/Desktop/net/Votes/");
        strcat(addr, filename);

        out.open(addr);
        if (!out.is_open())
            cout << "Error opening file!" << endl;
        
        for (int h = 0; h < allbox[k].candids.size(); h++)
        {
            out << allbox[k].candids[h].code << " " << allbox[k].candids[h].name << " " << allbox[k].candids[h].vote << endl;
        } 

        out.close();
    }

}

void print_result(vector<person> candids, vector<votebox> allbox)
{
    cout << ".::Overall::." << endl;
    for (int i = 0; i < candids.size(); ++i)
    {
        cout << candids[i].code << " " << candids[i].name << " " << candids[i].vote << endl;
    }
    cout << ":::::::::::::" << endl;
    



    for (int k = 1; k < allbox.size(); k++)
    {
        cout << ".::Box " << allbox[k].boxNum - 3 << "::." << endl;

        for (int h = 0; h < allbox[k].candids.size(); h++)
        {
            cout << allbox[k].candids[h].code << " " << allbox[k].candids[h].name << " " << allbox[k].candids[h].vote << endl;
        } 
        cout << ":::::::::::" << endl;
    }

}

bool exist(vector<person> candids, char* code)
{
    for (int k = 0; k < candids.size(); k++)
    {
        if (string(candids[k].code) == string(code))
        {
            return true;
        }
     
        
    }

    return false;
}


int create_socket(int port_number, char* host)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0)
    {
        char error[]="ERROR: NOT ABLE TO OPEN SOCKET!\n";
        write( 2, error, sizeof(error)-1);
        exit(1);
    }
    struct hostent *server;
    server = gethostbyname(host);
    if (server == NULL) {
        char error[]="ERROR: NO SUCH HOST!\n";
        write( 2, error, sizeof(error)-1);
        exit(1);
    }
    struct sockaddr_in server_address;
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;

    //copies length bytes from the first argumnet to the second argumnet
    
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port_number);
    if (connect(socket_fd,(struct sockaddr *) &server_address,sizeof(server_address)) < 0) 
    {
        char error[]="ERROR: NOT ABLE TO CONNECT!\n";
        write( 2, error, sizeof(error)-1);
        exit(1);
    }

    return socket_fd;
}

void write_socket(int socket_fd, char* buffer)
{
    int num_of_written = write(socket_fd,buffer,strlen(buffer));
    bzero(buffer,2000);
    
    if (num_of_written < 0) 
    {
        char error[]="ERROR: NOT ABLE TO WRITE TO SOCKET\n";
        write( 2, error, sizeof(error)-1);
        exit(1);   
    }
}

void read_socket(int socket_fd, char* buffer)
{
        bzero(buffer,2000);
        int num_of_read = read(socket_fd,buffer,255);
        if (num_of_read < 0) 
        {
            char error[]="ERROR: NOT ABLE TO READ FROM SOCKET\n";
            write( 2, error, sizeof(error)-1);
            exit(1);        
        }
        
        // write( 1, buffer, sizeof(buffer)-1);
        // write( 1, "\n", 1);

}

void *read_console(void*)
{
    int i = 0;
    char line[2000];
    char *command[20];
    char candidate[20];
    char token[20];
    char stime[6];
    char ftime[6];
    char etime[6];
    char rtime[] = "00:00";
    char code[3];


    read(0, line, sizeof(line));
    parse(line, command);

    strcpy(token, command[i++]);

    if (string(token) == "Add")
    {
        strcpy(candidate, command[i++]);
        strcpy(code, command[i++]);

        person p;

        if (!exist(candids, code))
        {
            strcpy(p.name, candidate);
            strcpy(p.code, code);
            p.vote = 0;

            candids.push_back(p);
        }
        else
            cout << "This code was used before!" << endl;


        for (int k = 1; k < allbox.size(); k++)
        {
            if (!exist(allbox[k].candids, code))
            {
                allbox[k].candids.push_back(p);

            }   
            
        }

    }    
    else if (string(token) == "Show")
    {
        strcpy(token, command[i++]);

        if (string(token) == "All")
        {
            strcpy(token, command[i++]);

            if (string(token) == "Results")
            {
                // CALL SHOW_RESULT FUNC
                print_result(candids, allbox);
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
    else if (string(token) == "Set")
    {
        strcpy(token, command[i++]);

        if (string(token) == "Voting")
        {
            strcpy(token, command[i++]);

            if (string(token) == "Time")
            {
                strcpy(stime, command[i++]);
                strcpy(ftime, command[i++]);
                // CALL SET_TIME FUNC
                set_voting_time(stime,ftime);
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
    else if (string(token) == "Extend")
    {
        strcpy(token, command[i++]);

        if (string(token) == "Voting")
        {
            strcpy(token, command[i++]);

            if (string(token) == "Time")
            {
                strcpy(etime, command[i++]);
                // CALL EXTEND_TIME FUNC
                extend(etime);
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

   // cout << "Hello World! Thread ID, " << line << endl;
   pthread_exit(NULL);
}





int main(int argc, char *argv[])
{

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    pthread_t th;
    int rc;
    // for (int i = 0; i < 100; ++i)
    // {
    //     rc = pthread_create(&th, NULL, &read_console, (void*)NULL);
    // }

    int returnval;
    int socket_fd;
    int new_fd;
    int max_fd;
    //*****************************
    fd_set overall_set,working_set;
    //*********************
    struct timeval timeout;
    //**********
    int closing;
    int length;
    int i;
    int on=1;
    int usrCount = 0;
    int j = 0;
    char ssn[20];
    char usr[20];
    char pass[20];
    char repass[20];
    char token[20];

    
    int ready_disc;
    int ending=0;
    struct sockaddr_in server_address;
    //check number of server arguments!
    if(argc < 2)
    {
        char error[]="ERROR: PORT WAS NOT SELECTED!\n";
        write( 2, error, sizeof(error)-1);
        exit(1);
    }
    //create a socket as an endpoint!
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0)
    {
        char error[]="ERROR: NOT ABLE TO OPEN SOCKET!\n";
        write( 2, error, sizeof(error)-1);
        exit(1);
    }
    //set all the server_address values to zero!
    //void bzero(void *s, size_t n)-----------> places n zero-valued bytes in the area pointed to by s.  
    bzero((char *) &server_address, sizeof(server_address));
    //get the port number which was an argument!
    int port_number = atoi(argv[1]);
    //set the values of the server_address!
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);
    //allow socket_fd to be reused!
    returnval=setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
    if(returnval < 0)
    {
        char error[]="ERROR: SETSOCKOPT FUNCTION FAILED!\n";
        write( 2, error, sizeof(error)-1);
        close(socket_fd);
        exit(1);
    }
    //set socket to be non-blocking!
    returnval=ioctl(socket_fd, FIONBIO, (char *)&on);
    if(returnval < 0)
    {
        char error[]="ERROR: IOCTL FUNCTION FAILED!\n";
        write( 2, error, sizeof(error)-1);
        close(socket_fd);
        exit(1);
    }
    //get the port!
    returnval=bind(socket_fd, (struct sockaddr *) &server_address,sizeof(server_address));
    if(returnval < 0) 
    {
        char error[]="ERROR: NOT BINDING!\n";
        write( 2, error, sizeof(error)-1);
        exit(1);
    }
    //set listening!
    returnval=listen(socket_fd,10);
    if(returnval < 0)
    {
        char error[]="ERROR: NOT LISTENNING!\n";
        write( 2, error, sizeof(error)-1);
        exit(1);
    }
    //set file descriptor sets values
    FD_ZERO(&overall_set);
    max_fd = socket_fd;
    FD_SET(0, &overall_set);
    FD_SET(socket_fd, &overall_set);
    timeout.tv_sec = 50000;

    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%
    votebox b;
    b.boxNum = socket_fd;

    for (int k = 0; k < candids.size(); k++)
    {
        person p;
        strcpy(p.name, candids[k].name);
        strcpy(p.code, candids[k].code);
        p.vote = 0;
        b.candids.push_back(p);
    }

    allbox.push_back(b);

    //a loop to look for connection or data of the connected clients
    do
    {
        memcpy(&working_set, &overall_set, sizeof(overall_set));
        returnval=select(max_fd+1, &working_set, NULL, NULL, &timeout);
        if(returnval < 0)
        {
            char error[]="ERROR: SELECT FUNCTION FAILED!\n";
            write( 2, error, sizeof(error)-1);
            exit(1);
        }
        if(returnval == 0)
        {
            char error[]="SELECT FUNCTION TIMED OUT!\n";
            write( 2, error, sizeof(error)-1);
            exit(1);            
        }
        ready_disc = returnval;
        for( i=0; i <= max_fd && ready_disc > 0; ++i)
        {

            if(FD_ISSET(i, &working_set))
            {
                ready_disc-=1;
                if(i==socket_fd)
                {
                    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                    


                    char written[]="LISTENNING SOCKET CAN READ!\n";
                    write( 1, written, sizeof(written)-1);
                    //accepting connections
                    do
                    {
                        new_fd = accept(socket_fd, NULL, NULL);
                        if( new_fd < 0)
                        {
                            if(errno != EWOULDBLOCK)
                            {
                                char error[]="ACCEPT FUNCTION FAILED!\n";
                                write( 2, error, sizeof(error)-1);
                                exit(1);  
                            }
                            break;
                        }
                        //add the new connection to the overall set
                        FD_SET(new_fd, &overall_set);
                        if(new_fd > max_fd)
                        {
                            max_fd = new_fd;
                        }

                        // ofstream fs;
                        // char box[5];
 

                        votebox b;
                        b.boxNum = new_fd;

                        for (int k = 0; k < candids.size(); k++)
                        {
                            person p;
                            strcpy(p.name, candids[k].name);
                            strcpy(p.code, candids[k].code);
                            p.vote = 0;
                            b.candids.push_back(p);
                        }

                        allbox.push_back(b);   

                    } while(new_fd != -1);
                }
                //data receive
                else
                {
                    closing = 0;
                    // do
                    // {
                        char buffer[2000];
                        bzero(buffer,2000);
                        if ( i == 0)
        				{
        					read(0, buffer, sizeof(buffer));
        					cout << "buffer is: " << buffer << endl;
        				}
        				else{
                        returnval = recv(i, buffer, sizeof(buffer), 0);
                        if(returnval < 0)
                        {
                            if(errno != EWOULDBLOCK)
                            {
                                char error[]="RECV FUNCTION FAILED!\n";
                                write( 2, error, sizeof(error)-1);
                            }
                            break;
                        }
                        if (returnval == 0)
                        {
                            char error[]="CONNECTON CLOSED!\n";
                            write( 2, error, sizeof(error)-1);
                            closing = 1;
                            break;
                        }

               // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

                        ofstream out;
                        ifstream in;
                        char *command[20];
                        char usr[20];
                        char code[3];
                        char certif[5];
                        char port_str[5];
                        char host[20];
                        char verify[2000];
                        char addr[50] = "/home/me/Desktop/net/Votes/";

                        bzero(usr,20);
                        bzero(code,3);
                        bzero(certif,5);
                        bzero(port_str,5);
                        bzero(host,20);
                        bzero(verify,2000);
                        bzero(token,20);

                        int ca_port_number;
                        int ca_socket;

                        j = 0;



                        parse(buffer, command);
                        strcpy(token, command[j++]);
                        // cout << "command is: " << token << endl;

                        if (string(token) == "Vote")
					    {

                            if(!validate_time())
                            {
                                cout<< "Voting time has finished!"<<endl;
                                exit(1);
                            }

					    	strcpy(usr , command[j++]);
					    	strcpy(code , command[j++]);
                            strcpy(certif , command[j++]);
					    	strcpy(host , command[j++]);
                            strcpy(port_str , command[j++]);

                            strcpy(verify, "Verify ");
                            strcat(verify, usr);
                            strcat(verify, " ");
                            strcat(verify, certif);
                            // cout << verify << endl;

                            ca_port_number = char_s_to_int(port_str);
                            ca_socket = create_socket(ca_port_number, host);
                            write_socket(ca_socket, verify);
                            read_socket(ca_socket, verify);
                            if (string(verify) == "true")
                            {

                                for (int k = 0; k < candids.size(); k++)
                                {
                                    if (string(candids[k].code) == string(code))
                                    {
                                        candids[k].vote = candids[k].vote + 1;
                                        break;
                                    }
                                }

                                for (int k = 0; k < allbox.size(); k++)
                                {
                                    if (allbox[k].boxNum == i)
                                    {
                                        for (int h = 0; h < allbox[k].candids.size(); h++)
                                        {
                                            if (string(allbox[k].candids[h].code) == string(code))
                                            {
                                                allbox[k].candids[h].vote = allbox[k].candids[h].vote + 1;
                                                break;
                                            }
                                        }   
                                    }
                                }


                            }
                            else
                            {
                                cout << "Authentication faild!";
                            }
                            close(ca_socket);

                            update_result(candids, allbox);
                            // cout << "Client " << i << ": "<< usr << " vote " << code << endl;

					    }

                        else if (string(token) == "Show")
                        {
                            bzero(buffer, 2000);
                            
                            strcpy(buffer,".::Candidates::.");
                            for (int k = 0; k < candids.size(); k++)
                            {
                                strcat(buffer, "\n");
                                strcat(buffer, candids[k].code);
                                strcat(buffer, " ");
                                strcat(buffer, candids[k].name);
                            }
                            strcat(buffer, "\n");
                            strcat(buffer, "::::::::::::::::\n");
                        }



                        length = returnval;
                        returnval = send(i, buffer, 2000, 0);
                        // bzero(buffer,2000);
                        if (returnval < 0)
                        {
                            char error[]="SEND FAILED!\n";
                            write( 2, error, sizeof(error)-1);
                            closing = 1;
                            break;
                        } 
                    }
                    // }while(1);
                    if(closing)
                    {
                        close(i);
                        FD_CLR(i, &overall_set);
                        if( i == max_fd )
                        {
                            while (FD_ISSET(max_fd, &overall_set) == 0)
                                max_fd -= 1;
                        }
                    }
                }
            }
        }
    }while(ending==0);
    for (i=0; i <= max_fd; ++i)
    {
        if (FD_ISSET(i, &overall_set))
            close(i);
    }
    return 0; 
}
