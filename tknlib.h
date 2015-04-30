#ifndef _TOKEN_
#define _TOKEN_

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

extern int  parse(char *line, char **words);
extern int length(char* s);
extern void itoa(int i,char* characters);
extern int char_s_to_int(char *char_s);
extern int create_socket(int port_number, char* host);
extern void write_socket(int socket_fd, char* buffer);
extern void read_socket(int socket_fd, char* buffer);
#endif