#ifndef _TOKEN_
#define _TOKEN_

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <map>

using namespace std;

extern int  parse(char *line, char **words);
extern int length(char* s);
extern void itoa(int i,char* characters);
extern int char_s_to_int(char *char_s);
extern void encaps(char* type, char* cmd, char* dst, char* src, char* data, char* packet);
extern int route(char* packet, char *port[], int portcount);
extern bool issw(char* packet);
extern bool iscl(char* packet);
extern bool issr(char* packet);
extern bool isfirst(char* packet);
#endif
