#include "tknlib.h"

int  parse(char *line, char **words)
{
	 int count = 0;
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *words++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
    count++; 
	}
     *words = '\0';                 /* mark the end of argument list  */
	return count-1;
}

int length(char* s)
{
	int endpoint = 0;
	while(s[endpoint])
		endpoint++;
	return endpoint;
}

void itoa(int i,char *characters)
{
    int place=0;
    int temp1=i;
    int temp2=i;
    int num_of_digits=0;
    memset(characters,'\0', sizeof(characters));
    while(temp1>=10)
    {
        temp1/=10;
        num_of_digits++;
    }
    num_of_digits++;
    place = num_of_digits-1;
    while(temp2>=10)
    {
        num_of_digits++;
        int remain = temp2%10;
        temp2/=10;
        characters[place]='0'+remain;
        place--;
    }
    if(temp2<10)
    {
        characters[place] = '0'+temp2; 
    }
    //return num_of_digits;
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
        int num_of_read = read(socket_fd,buffer,1999);
        if (num_of_read < 0) 
        {
            char error[]="ERROR: NOT ABLE TO READ FROM SOCKET\n";
            write( 2, error, sizeof(error)-1);
            exit(1);        
        }
        
        // write( 1, buffer, sizeof(buffer)-1);
        // write( 1, "\n", 1);

}
