#include "tknlib.h"

using namespace std;

void padding(char*  in, int len)
{
    int dif = len - strlen(in);
    char out[8];
    bzero(out,8);

    while(dif > 0){
        strcat(out, "0");
        dif--;
    }
    strcat(out, in);
    strcpy(in, out);
}


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

void encaps(const char* type, const char* cmd, const char* dst, const char* src, const char* data, char* packet)
{
 
    char ttl[] = "99";
    char crc[] = "111111";
    char length[] = "00128";
    strcpy(packet, type);
    strcat(packet, cmd);
    strcat(packet, dst);
    strcat(packet, src);
    strcat(packet, ttl);
    strcat(packet, length);
    strcat(packet, data);
    // strcat(packet, crc);

}

int route(char* packet, char *port[], int portcount)
{



}

bool issw(char* packet)
{
    char tmp[3];
    bzero(tmp, 3);
    copy(packet, packet+2, tmp);
    tmp[3] = '\0';

    if (string(tmp) == "sw" )
    {
        return true;
    }
    return false;
}

bool iscl(char* packet)
{
    char tmp[3];
    bzero(tmp, 3);
    copy(packet, packet+2, tmp);
    tmp[3] = '\0';

    if (string(tmp) == "cl" )
    {
        return true;
    }
    return false;
}

bool issr(char* packet)
{
    char tmp[3];
    bzero(tmp, 3);
    copy(packet, packet+2, tmp);
    tmp[3] = '\0';

    if (string(tmp) == "sr" )
    {
        return true;
    }
    return false;
}

bool issp(char* packet)
{
    char tmp[3];
    bzero(tmp, 3);
    copy(packet, packet+2, tmp);
    tmp[3] = '\0';

    if (string(tmp) == "sp" )
    {
        return true;
    }
    return false;
}

bool isfirst(char* packet)
{
    char tmp[3];
    bzero(tmp, 3);
    copy(packet+2, packet+4, tmp);
    tmp[3] = '\0';

    if (string(tmp) == "ct" )
    {
        return true;
    }
    return false;
}

bool reqlist(char* packet)
{
    char tmp[3];
    bzero(tmp, 3);
    copy(packet+2, packet+4, tmp);
    tmp[3] = '\0';

    if (string(tmp) == "gl" )
    {
        return true;
    }
    return false;
}

int getip(char* packet)
{
    char res[4];
    res[0] = packet[4];
    res[1] = packet[5];

    return atoi(res);
}

int getdst(char* packet)
{
    char res[8];
    bzero(res, 8);
    copy(packet+4, packet+12, res);
    return atoi(res);
}

int getsrc(char* packet)
{
    char res[8];
    bzero(res, 8);
    copy(packet+12, packet+20, res);
    return atoi(res);
}

int getcmd(char* packet)
{
    char res[3];
    bzero(res, 3);
    copy(packet+2, packet+4, res);
    res[3] = '\0';
    if (string(res) == "rd")
        return 1;
    if (string(res) == "ap")
        return 2;
    if (string(res) == "wr")
        return 3;
}

int ttl(char* packet)
{
    char res[2];
    bzero(res, 2);
    copy(packet+20, packet+22, res);
    return atoi(res);
}

void chdst(char* buf, char* dst)
{
    char tmp[116];
    char packout[128];
    bzero(tmp,116);
    bzero(packout,128);
    copy(buf+12, buf+128, tmp);
    copy(buf, buf+4, packout);
    strcat(packout, dst);
    strcat(packout, tmp);
    bzero(buf, 128);
    copy(packout, packout+128, buf);
}

void chsrc(char* buf, char* src)
{
    char tmp[108];
    char packout[128];
    bzero(tmp,108);
    bzero(packout,128);
    copy(buf+20, buf+128, tmp);
    strncpy(packout, buf, 12);
    strcat(packout, src);
    strcat(packout, tmp);
    bzero(buf, 128);
    strcpy(buf, packout);
}

void chtype(char* buf, char* type)
{
    char tmp[126];
    bzero(tmp,126);
    copy(buf+2, buf+128, tmp);
    bzero(buf, 128);
    strcpy(buf, type);
    strcat(buf, tmp);

}

void chcmd(char* buf, char* cmd)
{
    char tmp[124];
    char type[2];
    bzero(tmp,124);
    bzero(type,2);
    copy(buf, buf+2, type);
    copy(buf+4, buf+128, tmp);
    bzero(buf, 128);
    strcpy(buf, type);
    strcat(buf, cmd);
    strcat(buf, tmp);

}

void chdata(char* buf, char* data)
{
    char tmp[28];
    bzero(tmp,28);
    copy(buf, buf+27, tmp);
    bzero(buf, 128);
    strcpy(buf, tmp);
    strcat(buf, data);
}

void decttl(char* buf)
{
    int ttl;
    char tmp[2];
    char pre[20];
    char post[106];
    bzero(tmp, 2);
    bzero(pre, 20);
    bzero(post, 106);
    copy(buf+20, buf+22, tmp);
    ttl = atoi(tmp);
    ttl--;
    itoa(ttl, tmp);
    copy(buf, buf+20, pre);
    copy(buf+22, buf+128, post);
    bzero(buf,128);
    strcpy(buf, pre);
    strcat(buf, tmp);
    strcat(buf, post);
}
