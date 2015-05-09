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
extern void padding(char*  in, int len);
extern int  parse(char *line, char **words);
extern int length(char* s);
extern void itoa(int i,char* characters);
extern int char_s_to_int(char *char_s);
extern void encaps(const char* type, const char* cmd, const char* dst, const char* src, const char* data, char* packet);
extern int route(char* packet, char *port[], int portcount);
extern bool issw(char* packet);
extern bool iscl(char* packet);
extern bool issr(char* packet);
extern bool issp(char* packet);
extern bool isfirst(char* packet);
extern int getdst(char* packet);
extern int getsrc(char* packet);
extern void chdst(char* buf, char* dst);
extern void chsrc(char* buf, char* src);
extern void chtype(char* buf, char* type);
extern void chdir(char* buf);
#endif
